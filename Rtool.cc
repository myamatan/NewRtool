#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <string.h>  
#include <ctype.h>
#include <termios.h>  
#include <time.h>  
#include <sys/wait.h>  
#include <sys/time.h>  

#include <sys/ioctl.h>
#include <system_error>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <arpa/inet.h>

#include <math.h>

#include <sstream>
#include <iostream>
#include <algorithm>

#include <TApplication.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TText.h>
#include <TStyle.h>

#include <TMath.h>

#include <TGClient.h>
#include <TGLabel.h>
#include <TGTab.h>
#include <TGButton.h>
#include <TGNumberEntry.h>
#include <TGButtonGroup.h>
#include <TGFrame.h>
#include <TGLayout.h>
#include <TGWindow.h>
#include <TGStatusBar.h>
#include <TGTab.h>
#include <TG3DLine.h>
#include <TGComboBox.h>
#include <TGString.h>

#include <TThread.h>
#include <TMutex.h>

#include <TRandom.h>

#include <fstream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TH1.h"
#include "TGaxis.h"
#include "TGraphErrors.h"
#include "TLegend.h"

#include <TGPicture.h>
#include <TGMenu.h>
#include <TGImageMap.h>
#include <TGMsgBox.h>

#include "Rtool.hh"
#include "Variables.hh"

#define BAUDRATE B115200  

#include "Relay.hh"
#include "rcomparison.hh"
#include "Config_reader.hh"

static int fd = -1;  
bool lock=false;

Rtool* monitor=0;
Relay* relay=0;

#include "RtoolFunction.hh"

int charToInt(char low, char high){
  int v = (unsigned char)high;
  v = (v<<8)+(unsigned char)low;
  return v;
}

void serial_init() {  
  fd = open(device, O_RDWR | O_NOCTTY);
  if (fd < 0) {
    perror(device);
    if(monitor) monitor->Finalize();
    exit(1);
  }

  struct termios tio;  
  memset(&tio, 0, sizeof(tio));  

  tio.c_cflag = CS8 | CLOCAL | CREAD;  
  tio.c_cc[VTIME] = 0;
      
  //tio.c_lflag = ICANON;  
  tio.c_iflag = IGNPAR | ICRNL;  
      
  cfsetispeed(&tio, BAUDRATE);  
  cfsetospeed(&tio, BAUDRATE);  
  tcsetattr(fd, TCSANOW, &tio);  

  // kara-yomi
  char buf[BUFFSIZE];
  int n;
 
  do {
    usleep(200*1000);
    n = read (fd, buf, sizeof buf);
  } while (n > 0);

  //tio.c_cc[VMIN] = 1; // require at least 1
  //tcsetattr(fd, TCSANOW, &tio);  
}  

void serial_close() {
  close(fd);
}

double stof(const std::string& str) {
  std::stringstream ss(str);
  double f;
  ss >> f;
  return f;
}

std::string getVal(const std::string query){
  while(lock) usleep(1000); lock=true;
  //std::cout << "### read(" << query.length() << ")=" << query << std::endl;
  int count = write(fd, query.c_str(), query.length());
  char buf[BUFFSIZE];
  usleep(50*1000);
  count = read(fd, &buf, BUFFSIZE);  
  //std::cout << "### response(" << count << ")=" << buf << std::endl;
  // CMD_ACK<CR>VALUES<CR>
  // CMD_ACK=0:OK ,1:syntax error, 2:execution error, 5:no data available
  if (count<2 || int(buf[0])!=48 || (int(buf[1])!=10 && int(buf[1])!=13) || (int(buf[count-1])!=10 && int(buf[count-1])!=13)) {//'0'=48, '\r'=10
    lock=false;
    return "";
  }
  std::string str(buf+2,count-3);
  lock=false;
  return str;
}

bool getID(std::string& model, std::string& ver, std::string& serial){
  std::string str = getVal("id\r");
  if(str=="") return false;
  std::replace(str.begin(), str.end(), ' ', '-');
  std::replace(str.begin(), str.end(), ',', ' ');
  std::istringstream ist(str);
  ist >> model >> ver >> serial;
  //std::cout << "model=" << model << " ver=" << ver << " serial=" << serial << std::endl;
  return true; 
}

bool getData(std::string& range, std::string& unit, std::string& state, std::string& attr){
  std::string str = getVal("qm\r");
  if(str=="") return false;
  std::replace(str.begin(), str.end(), ',', ' ');
  std::istringstream ist(str);
  ist >> range >> unit >> state >> attr;
  //std::cout << "range=" << range << " unit=" << unit << " state=" << state << " attr=" << attr << std::endl;
  return true; 
}

void handler (int) {
  if(monitor) monitor->Finalize();
  exit(1);
}


void Rtool::PrintCanvas(std::string name){
  if(name.find(".root")!=std::string::npos)
    name = name.substr(0,name.find(".root"));
  if(canv){
    canv->Print((name+".gif").c_str());
    canv->Print((name+".C").c_str());
  } 
}

void Rtool::ChangeNDisp(){
  ndisp = tg_ndisp->GetIntNumber();
}
void Rtool::ChangeRangeV(){
  range_v[0]=tg_rangeVMin->GetNumber();
  range_v[1]=tg_rangeVMax->GetNumber();
}

void Rtool::UpdatePanel(){
  time = (event_time.tv_sec-time_start.tv_sec)*1000 + (event_time.tv_usec-time_start.tv_usec)/1000;//ms

  char text_time[BUFFSIZE];
  sprintf(text_time,"%ld",time/1000);
  tg_elapse->SetText(text_time);

  strftime(text_time,20,"%y/%m/%d %T",localtime(&event_time.tv_sec));
  tg_currentDate->SetText(text_time);

  if(time-time_rate>1000.*5){//5s
    double rate = (n-n_rate)*1000.0/(time-time_rate); 
    n_rate=n;
    time_rate=time; 
    sprintf(text_time,"%.2f",rate);
    tg_rate->SetText(text_time);
  }
  
  sprintf(text_time,"%ld",n);
  tg_counter->SetText(text_time);

  char text_sb[BUFFSIZE];
  sprintf(text_sb,"value= %s %s",str_value.c_str(),str_unit.c_str());
  tg_sb->SetText(text_sb,0);
  sprintf(text_sb,"state= %s",str_state.c_str());
  tg_sb->SetText(text_sb,1);
  sprintf(text_sb,"attr= %s",str_attr.c_str());
  tg_sb->SetText(text_sb,2);
  
  sprintf(text_sb,"elapse= %ldd %ldh %ldm %lds",((time/1000)/60/60/24)%365,((time/1000)/60/60)%24,((time/1000)/60)%60,(time/1000)%60);
  tg_sb->SetText(text_sb,3);

}

void Rtool::UpdateCanvas(){

  double start =0;
  double end =time/1000.;
  if(end-start>ndisp) start=end-ndisp;

  gPad->SetLeftMargin(0.08);
  gPad->SetRightMargin(0.05);
  if(g_frame_v) {
    delete g_frame_v;
  }
  g_frame_v = new TGraph(2,x_frame_v,y_frame_v);
  g_frame_v->SetName("g_frame_v");
  g_frame_v->SetLineColor(0);
  g_frame_v->SetTitle("fluke monitor");
  if(!tg_date->IsOn()) g_frame_v->GetXaxis()->SetTitle("[s]");
  char text_unit[BUFFSIZE];
  sprintf(text_unit,"[%s]",str_unit.c_str());
  g_frame_v->GetYaxis()->SetTitle(text_unit);
  g_frame_v->GetXaxis()->SetTitleOffset(0.7);
  g_frame_v->GetYaxis()->SetTitleOffset(0.6);
  g_frame_v->GetXaxis()->SetTitleSize(0.05);
  g_frame_v->GetYaxis()->SetTitleSize(0.05);
  g_frame_v->GetXaxis()->SetLabelSize(0.07);
  g_frame_v->GetYaxis()->SetLabelSize(0.07);
  g_frame_v->GetXaxis()->SetRangeUser(start,end+(end-start)*0.02);
  g_frame_v->GetYaxis()->SetRangeUser(range_v[0],range_v[1]);
  if(tg_date->IsOn()) g_frame_v->GetXaxis()->SetTimeDisplay(1);
  g_frame_v->Draw("AL");
  gPad->SetGrid();
  
  
  TText t;
  char text[BUFFSIZE];

  if(g_v) delete g_v;
  g_v = new TGraph(n_v,x_v,y_v);//todo: use Expand and SetPoint for the same object
  g_v->SetName("g_v");
  g_v->SetMarkerStyle(7);
  g_v->SetMarkerColor(1);
  g_v->SetLineWidth(2);
  g_v->SetLineColor(1);
  g_v->Draw("LP");
  
  sprintf(text,"%.3f %s",y_v[n_v-1],str_unit.c_str());
  if(y_v[n_v-1]<1e-0) 
    sprintf(text,"%.3fE-3 %s",y_v[n_v-1]*1e+3,str_unit.c_str());
  if(y_v[n_v-1]>1e+3) 
    sprintf(text,"%.3fE+3 %s",y_v[n_v-1]*1e-3,str_unit.c_str());
  if(y_v[n_v-1]>1e+6) 
    sprintf(text,"%.3fE+6 %s",y_v[n_v-1]*1e-6,str_unit.c_str());
  if(fabs(y_v[n_v-1])>1e+37) 
    sprintf(text,"overflow %s",str_unit.c_str());
  t.SetTextColor(2);
  t.SetTextSize(0.07);
  t.DrawTextNDC(0.15,0.8,text);

  canv->Update();
}

void Rtool::Update(){
 while(1){
 if(timerOn){
  gettimeofday(&event_time,NULL);
  time = (event_time.tv_sec-time_start.tv_sec)*1000 + (event_time.tv_usec-time_start.tv_usec)/1000;//ms

  time_diff = time - time_prev;
  time_prev=time;

  // frame
  if(n==0) {
    x_frame_v[0]=time/1000.;
    y_frame_v[0]=range_v[0];
  } else {
    x_frame_v[1]=time/1000.;
    y_frame_v[1]=range_v[1];
  } 

  //// Trigger for data taking
  bool ret = getData(str_value,str_unit,str_state,str_attr); 
  if(ret) value = std::stof(str_value);
  else value = -9.99999999E+37;

  //value = gRandom->Gaus(0.4, 0.2) * 1e+6; // for demonstration

  // Identify Standard deviation of least five measurement as Data taking trigger
  for(int s=0; s<5; s++){
	  R[s] = R[s+1];
	  R2[s] = R2[s+1];
  }
  R[4] = value;
  R2[4] = value*value;
  double sum = R[0]+R[1]+R[2]+R[3]+R[4];
  double sum2 = R2[0]+R2[1]+R2[2]+R2[3]+R2[4];
  STD = sqrt( (sum2-sum*sum/5)/4 );
  if( checkButtonAct  && STD < 0.5e+6 && sum/5 < 1.0e+8 ) {//1e+7, 1e+7
	  tempRN = tempRN + 1.0;
  }
  ////

  static const char* UNIT[] = {"NONE","VDC","VAC","ADC","AAC",
                         "VAC_PLUS_DC","AAC_PLUS_DC","V","A","OHM",
                         "SIE","Hz","S","F","CEL",
                         "FAR","PCT","dBm","dBV","dB",
                         "CREST_FACTOR"};
  for(int i=0; i<21; i++){
    if(str_unit==UNIT[i]){
      unit=i;
      break;
    }
    if(i==21-1) unit=21;
  }

  static const char* STATE[] = {"INVALID","NORMAL","BLANK","DISCHARGE","OL",
                         "LD_MINUS","OPEN_TC"};
  for(int i=0; i<7; i++){
    if(str_state==STATE[i]){
      state=i;
      break;
    }
    if(i==7-1) state=7;
  }

  static const char* ATTR[] = {"NONE","OPEN_CIRCUIT","SHORT_CIRCUIT","GLITCH_CIRCUIT","GOOD_DIODE",
                         "LO_OHMS","NEGATIVE_EDGE","POSITIVE_ELDGE","HIGH_CURRENT"};
  for(int i=0; i<9; i++){
    if(str_attr==ATTR[i]){
      attr=i;
      break;
    }
    if(i==9-1) attr=9;
  }



  if(n_v>=3 && y_v[n_v-2]==y_v[n_v-1] && y_v[n_v-2]==value)
    x_v[n_v-1]=time/1000.;
  else {
    x_v[n_v]=time/1000.;
    y_v[n_v]=value;
    n_v +=1;
  }

  n++;

  bool stop=(n>=nmax);
  stop|= (n_v>=nmax/fv);

  if(stop) {
    std::cout << "stop" << std::endl;
    thread->Kill();
    usleep(200*1000);
    serial_close();
  } 
 }
  TThread::CancelPoint();
  //usleep(500*1000);//2Hz
  //usleep(200*1000);//5Hz
  usleep(5*1000);//500Hz
  //usleep(1*1000);//100kHz
 }
}

void Rtool::Finalize(){
  if(thread) {
    thread->Kill();
  }
  usleep(700*1000);
  timerPanel->TurnOff();
  timerCanvas->TurnOff();
  timerCanvasOn=false;
  
}

void Rtool::PauseCanvas(){
  if(timerCanvasOn){
    timerCanvas->TurnOff();
    timerCanvasOn=false;
    //update->SetOn(false);
  }else{
    timerCanvas->TurnOn();
    timerCanvasOn=true;
    //update->SetOn(true);
  }
}


void Rtool::Pause(){
  bool stop=(n>=nmax);
  stop|= (n_v>=nmax/fv);

  if(stop) return;
  if(timerOn){
    tg_pause->SetText("Resume");
    timerOn=false;
    //thread->Kill();
    usleep(200*1000);
    serial_close();
  }else{
    tg_pause->SetText("Pause");
    serial_init();  
    //thread->Run();
    timerOn=true;
  }
}


void Rtool::CloseWindow(){
  Finalize();
  gApplication->Terminate();
  //TGMainFrame::CloseWindow();
}

void Rtool::relaySwitch(){
	int left = tg_relayLeft->GetNumber();
	int right = tg_relayRight->GetNumber();
	relay->change(left, right);
}	
void Rtool::SetOverWrite(){
	if( ifOverWrite ) ifOverWrite = false;
	else ifOverWrite = true;
}
void Rtool::SetRemeasure(){
	if( isRemeasure ) isRemeasure = false;
	else isRemeasure = true;
}

void Rtool::SetRemeasure_HV(){
	if( isRemeasure_HV ) isRemeasure_HV = false;
	else isRemeasure_HV = true;
}
void Rtool::SetShifter(){
	if( isShifter ) isShifter = false;
	else isShifter = true;
}

void Rtool::BackRange(){
	if( !save_check ){
		TString Type =  tg_fileType->GetText();
		std::vector<std::vector<int> > vec = positionList[Type];	
		scanCount--;
		if( scanCount < 0 ) scanCount = 0;
		tg_gOMrange->SetNumber(vec[scanCount][1]); 
		tg_gOMrange0->SetNumber(vec[scanCount][0]);	
	}
}

void Rtool::NextRange(){
	if( !save_check ){
		TString Type =  tg_fileType->GetText();
		std::vector<std::vector<int> > vec = positionList[Type];	
		scanCount++;
		if( scanCount >= vec.size() ) scanCount = vec.size()-1;
		tg_gOMrange->SetNumber(vec[scanCount][1]); 
		tg_gOMrange0->SetNumber(vec[scanCount][0]);	
	}
}

Rtool::~Rtool(){
}

Rtool::Rtool(): TGMainFrame(gClient->GetRoot(), 10, 10, kHorizontalFrame) {
  ndisp=nmax/2;//~2Hz

  n = 0;
  timerOn=false;

  range_v[0] = -10;
  range_v[1] = 10;

  g_frame_v=0;
  g_v=0;

  n_v=0;

  serial_init();  

  signal(SIGINT, handler);
  signal(SIGTERM,handler);
  //signal(SIGHUP, handler);

   TGVerticalFrame *controls = new TGVerticalFrame(this);
   AddFrame(controls, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 
                                        5, 5, 5, 5));

  std::string model,ver,serial;
  bool ret = getID(model,ver,serial);
  if(ret==false){
    model="---";
    ver="---";
    serial="---";
  }
   char text_lab[BUFFSIZE];
   sprintf(text_lab,"model: %s",model.c_str());
   tg_lab1 = new TGLabel(controls,text_lab);
   tg_lab1->SetMargins(30,30,2,0);
   tg_lab1->SetTextFont(gClient->GetFont("-*-helvetica-bold-r-*-*-14-*-*-*-*-*-*-*"));
   tg_lab1->SetTextColor(gROOT->GetColor(2));
   controls->AddFrame(tg_lab1, new TGLayoutHints(kLHintsLeft, 0, 2, 2, 2));

   sprintf(text_lab,"ver: %s serial: %s",ver.c_str(),serial.c_str());
   tg_lab2 = new TGLabel(controls,text_lab);
   tg_lab2->SetMargins(30,30,2,0);
   tg_lab2->SetTextFont(gClient->GetFont("-*-helvetica-bold-r-*-*-12-*-*-*-*-*-*-*"));
   tg_lab2->SetTextColor(gROOT->GetColor(4));
   controls->AddFrame(tg_lab2, new TGLayoutHints(kLHintsLeft, 0, 2, 2, 2));
 
   /*
   TGGroupFrame *logoframe = new TGGroupFrame(controls, "");
   TGImageMap* logoImage = new TGImageMap(logoframe, "../Images/icepp-logo-colorB.png");
   logoframe->AddFrame(logoImage);
   TGDimension size = logoframe->GetDefaultSize();
   logoframe->Resize(size);
   logoframe->MapSubwindows();
   controls->AddFrame(logoframe);
   controls->MapRaised();
*/


   TGTab* tab=new TGTab(controls,10,10);
   controls->AddFrame(tab, new TGLayoutHints(kLHintsLeft | kLHintsExpandY,
                                        5, 5, 5, 5));

   TGCompositeFrame*tab1 = tab->AddTab("Fluke");  
   TGCompositeFrame*tab2 = tab->AddTab("Measurement");  

   TGVerticalFrame *controls1 = new TGVerticalFrame(tab1);
   tab1->AddFrame(controls1, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 
                                        5, 5, 5, 5));
   TGVerticalFrame *controls2 = new TGVerticalFrame(tab2);
   tab2->AddFrame(controls2, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 
                                        5, 5, 5, 5));


   TGVertical3DLine *separator = new TGVertical3DLine(this);
   AddFrame(separator, new TGLayoutHints(kLHintsLeft | kLHintsExpandY));

   TGVerticalFrame *contents = new TGVerticalFrame(this);
   AddFrame(contents, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,5,5));
  
   //// Canvas tab 
   tg_tab_contents = new TGTab(contents,10,10);
   contents->AddFrame(tg_tab_contents, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY,
                                         5, 5, 5, 5));  
   /// fluke monitor canvas
   TGCompositeFrame* tab_fluke = tg_tab_contents->AddTab("fluke");  
   
   /// Distribution and Output canvas
   TGCompositeFrame* tab_Dist = tg_tab_contents->AddTab("Result");  
   TRootEmbeddedCanvas* ecanv_Dist = new TRootEmbeddedCanvas("Result",tab_Dist,750,500);
   canv_Dist = ecanv_Dist->GetCanvas();
   canv_Dist->Divide(1,2);
   tab_Dist->AddFrame(ecanv_Dist, new TGLayoutHints(kLHintsExpandX|kLHintsExpandY,
      			   5, 5, 5, 5));
   /// Pre Distribution canvas (KOBE)
   TGCompositeFrame* tab_preDist = tg_tab_contents->AddTab("PreResult");  
   TRootEmbeddedCanvas* ecanv_preDist = new TRootEmbeddedCanvas("preResult",tab_preDist,750,500);
   canv_preDist = ecanv_preDist->GetCanvas();
   canv_preDist->Divide(1,2);
   tab_preDist->AddFrame(ecanv_preDist, new TGLayoutHints(kLHintsExpandX|kLHintsExpandY,
      			   5, 5, 5, 5));
   /// HVline canvas 
   TGCompositeFrame* tab_HVline = tg_tab_contents->AddTab("HV line");  
   TRootEmbeddedCanvas* ecanv_HVline = new TRootEmbeddedCanvas("HV line",tab_HVline,750,500);
   canv_HVline = ecanv_HVline->GetCanvas();
   tab_HVline->AddFrame(ecanv_HVline, new TGLayoutHints(kLHintsExpandX|kLHintsExpandY,
      			   5, 5, 5, 5));
   /// Calibration canvas 
   TGCompositeFrame* tab_Calib = tg_tab_contents->AddTab("Calibration");  
   TRootEmbeddedCanvas* ecanv_Calib = new TRootEmbeddedCanvas("Calibration",tab_Calib,750,500);
   canv_Calib = ecanv_Calib->GetCanvas();
   tab_Calib->AddFrame(ecanv_Calib, new TGLayoutHints(kLHintsExpandX|kLHintsExpandY,
      			   5, 5, 5, 5));
   
   /// for fluke monitor .... 
   TRootEmbeddedCanvas* ecanv = new TRootEmbeddedCanvas("Rtool_monitor",tab_fluke,750,500);
   canv = ecanv->GetCanvas();
   tab_fluke->AddFrame(ecanv, new TGLayoutHints(kLHintsExpandX|kLHintsExpandY,
      			   5, 5, 5, 5));

   /*
   TRootEmbeddedCanvas* ecanv = new TRootEmbeddedCanvas("Rtool_monitor",contents,750,500);
   canv = ecanv->GetCanvas();
   contents->AddFrame(ecanv, new TGLayoutHints(kLHintsExpandX|kLHintsExpandY,
			   5, 5, 5, 5));
   */
  
   tg_sb = new TGStatusBar(contents, 750, 7);
   Int_t parts[] = {25,25,25,25};
   tg_sb->SetParts(parts,4);
   contents->AddFrame(tg_sb, new TGLayoutHints(kLHintsBottom|kLHintsExpandX, 2, 2, 2, 2));

   /// Fluke Tab 
   // update display
   tg_update = new TGCheckButton(controls1, "Update Display");
   controls1->AddFrame(tg_update, new TGLayoutHints(kLHintsExpandX, 0, 0, 2, 2));
   tg_update->SetOn();
   tg_update->Connect("Clicked()","Rtool",this,"PauseCanvas()");

   // date time
   tg_date = new TGCheckButton(controls1, "Use Date");
   controls1->AddFrame(tg_date, new TGLayoutHints(kLHintsExpandX, 0, 0, 2, 2));
   tg_date->SetOn(false);


   // Ndisp
   NumberFieldInt *NDisp = new NumberFieldInt(controls1, "display period[s]",-1,ndisp,nmax);
   controls1->AddFrame(NDisp, new TGLayoutHints(kLHintsRight, 0, 20, 2, 2));
   NDisp->GetEntry()->GetNumberEntry()->Connect("ReturnPressed()", "Rtool",
                             this, "ChangeNDisp()");
   tg_ndisp = NDisp->GetEntry();


   // dates
   TextField* startDate = new TextField(controls1, "start",0,"-");
   controls1->AddFrame(startDate, new TGLayoutHints(kLHintsRight, 0, 20, 2, 2));
   tg_startDate = startDate->GetEntry();

   TextField* currentDate = new TextField(controls1, "current",0,"-");
   controls1->AddFrame(currentDate, new TGLayoutHints(kLHintsRight, 0, 20, 2, 2));
   tg_currentDate = currentDate->GetEntry();

   TextField* elapse = new TextField(controls1, "elapse(s)",0,"-");
   controls1->AddFrame(elapse, new TGLayoutHints(kLHintsRight, 0, 20, 2, 2));
   tg_elapse = elapse->GetEntry();

   TextField* counter = new TextField(controls1, "#event",0,"0");
   controls1->AddFrame(counter, new TGLayoutHints(kLHintsRight, 0, 20, 2, 2));
   tg_counter = counter->GetEntry();

   TextField* rate = new TextField(controls1, "rate(Hz)",0,"-");
   controls1->AddFrame(rate, new TGLayoutHints(kLHintsRight, 0, 20, 2, 2));
   tg_rate = rate->GetEntry();



   // Setting range 
   TGGroupFrame *ranges = new TGGroupFrame(controls1, "setting Range Y");
   ranges->SetTitlePos(TGGroupFrame::kCenter);

   TGVerticalFrame *ranges_v = new TGVerticalFrame(ranges);
   ranges->AddFrame(ranges_v, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,0,0));

   NumberFieldReal *vMax = new NumberFieldReal(ranges_v, "max.  ",0,range_v[1],500e+6,15,10,TGNumberFormat::kNESRealThree, TGNumberFormat::kNEAAnyNumber,-500e+6);
   ranges_v->AddFrame(vMax, new TGLayoutHints(kLHintsRight, 0, 0, 2, 2));
   vMax->GetEntry()->GetNumberEntry()->Connect("ReturnPressed()", "Rtool", this, "ChangeRangeV()");
   tg_rangeVMax = vMax->GetEntry();

   NumberFieldReal *vMin = new NumberFieldReal(ranges_v, "min.  ",0,range_v[0],500e+6,15,10,TGNumberFormat::kNESRealThree, TGNumberFormat::kNEAAnyNumber,-500e+6);
   ranges_v->AddFrame(vMin, new TGLayoutHints(kLHintsRight, 0, 5, 2, 2));
   vMin->GetEntry()->GetNumberEntry()->Connect("ReturnPressed()", "Rtool", this, "ChangeRangeV()");
   tg_rangeVMin = vMin->GetEntry();

   controls1->AddFrame(ranges, new TGLayoutHints(kLHintsExpandX, 0, 0, 2, 2));
   
   // Relay control
   TGGroupFrame *relay_moni = new TGGroupFrame(controls1, "Relay control");
   relay_moni->SetTitlePos(TGGroupFrame::kCenter);

   TGVerticalFrame *relay_moni_v = new TGVerticalFrame(relay_moni);
   relay_moni->AddFrame(relay_moni_v, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,0,0));

   NumberFieldReal *relay_left = new NumberFieldReal(relay_moni,"Left. ",0,1,100,15,10,TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber,0);
   relay_moni->AddFrame(relay_left, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
   relay_left->GetEntry()->GetNumberEntry()->Connect("ReturnPressed()", "Rtool", this, "relaySwitch()");
   tg_relayLeft = relay_left->GetEntry();
   
   NumberFieldReal *relay_right = new NumberFieldReal(relay_moni,"Right. ",0,10,100,15,10,TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber,0);
   relay_moni->AddFrame(relay_right, new TGLayoutHints(kLHintsLeft, 0, 5, 2, 2));
   relay_right->GetEntry()->GetNumberEntry()->Connect("ReturnPressed()", "Rtool", this, "relaySwitch()");
   tg_relayRight = relay_right->GetEntry();
  
   controls1->AddFrame(relay_moni, new TGLayoutHints(kLHintsExpandX, 0, 0, 2, 2));

   /// Measurement Tab
   // Write filename
   TGGroupFrame *filename = new TGGroupFrame(controls2, "Output file's name");
   filename->SetTitlePos(TGGroupFrame::kCenter);
   
   TGVerticalFrame *filename_v = new TGVerticalFrame(filename);
   filename->AddFrame(filename_v, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,0,0));
   
   //TGLabel *label_combo = new TGLabel(filename_v, "Type.");
   //filename_v->AddFrame(label_combo, new TGLayoutHints(kLHintsLeft,2));
  
   fCombo = new TGComboBox(filename_v, "Select type", 1);
   for (UInt_t i=0; i<typeList.size(); i++) {
	   fCombo->AddEntry(TString(typeList.at(i)), i+1);
   }
   filename_v->AddFrame(fCombo, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsLeft,0,0,2,2));
   tg_fileType = fCombo->GetTextEntry();
   
   write_fileID = new NumberFieldReal(filename_v,"Foil ID",1,0,86,15,10,TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber,0);
   write_pcbID = new NumberFieldReal(filename_v,"PCB ID",1,0,86,15,10,TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber,0);
   if( QC_loc == "CERN" ) filename_v->AddFrame(write_pcbID, new TGLayoutHints(kLHintsExpandY | kLHintsRight, 0, 0, 2, 2));
   filename_v->AddFrame(write_fileID, new TGLayoutHints(kLHintsExpandY | kLHintsRight, 0, 0, 2, 2));
   tg_fileID = write_fileID->GetEntry();
   tg_pcbID = write_pcbID->GetEntry();
   
   TGHorizontalFrame *filename_h = new TGHorizontalFrame(filename);
   filename->AddFrame(filename_h, new TGLayoutHints(kLHintsExpandX,0,0));
   
   TGTextButton *tg_ok_filename = new TGTextButton(filename_h, "Create");
   filename_h->AddFrame(tg_ok_filename, new TGLayoutHints(kLHintsRight,
                                              0, 0, 0, 2));
   tg_ok_filename->Connect("Pressed()", "Rtool", this, "SetNewfile()");

   tg_overwrite = new TGCheckButton(filename_h, "Overwrite");
   filename_h->AddFrame(tg_overwrite, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
   tg_overwrite->SetOn(false);
   tg_overwrite->Connect("Clicked()","Rtool",this,"SetOverWrite()");
  
   controls2->AddFrame(filename, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));

   // GetOhmMap
   TGGroupFrame *tg_gOM = new TGGroupFrame(controls2, "Get OhmMap");
   tg_gOM->SetTitlePos(TGGroupFrame::kCenter);
   
   //TGVerticalFrame *gOM_h0 = new TGVerticalFrame(tg_gOM);
   TGHorizontalFrame *gOM_h0 = new TGHorizontalFrame(tg_gOM);
   tg_gOM->AddFrame(gOM_h0, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,0,0));
   
   NumberFieldReal *gOMrange0 = new NumberFieldReal(gOM_h0,"from(1~)",1,1,10,3,2,TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber,0);
   gOM_h0->AddFrame(gOMrange0, new TGLayoutHints(kLHintsExpandY | kLHintsLeft, 0, 0, 2, 2));
   tg_gOMrange0 = gOMrange0->GetEntry();
   
   NumberFieldReal *gOMrange = new NumberFieldReal(gOM_h0,"to(~10)",1,1,10,3,2,TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber,0);
   gOM_h0->AddFrame(gOMrange, new TGLayoutHints(kLHintsExpandY | kLHintsRight, 0, 0, 2, 2));
   tg_gOMrange = gOMrange->GetEntry();
   
   TGHorizontalFrame *gOM_h = new TGHorizontalFrame(tg_gOM);
   tg_gOM->AddFrame(gOM_h, new TGLayoutHints(kLHintsExpandX,0,0));
   
   TGTextButton *tg_gOMmeas = new TGTextButton(gOM_h, "Measure");
   gOM_h->AddFrame(tg_gOMmeas, new TGLayoutHints(kLHintsRight,
                                              0, 0, 0, 2));
   tg_gOMmeas->Connect("Pressed()", "Rtool", this, "getOhmMap()");
   
   TGTextButton *tg_gOMnext = new TGTextButton(gOM_h, "Next");
   gOM_h->AddFrame(tg_gOMnext, new TGLayoutHints(kLHintsRight,
                                              0, 0, 0, 2));
   tg_gOMnext->Connect("Pressed()", "Rtool", this, "NextRange()");
   
   TGTextButton *tg_gOMback = new TGTextButton(gOM_h, "Back");
   gOM_h->AddFrame(tg_gOMback, new TGLayoutHints(kLHintsRight,
                                              0, 0, 0, 2));
   tg_gOMback->Connect("Pressed()", "Rtool", this, "BackRange()");
   
   
   tg_remeasure = new TGCheckButton(gOM_h, "re-try");
   gOM_h->AddFrame(tg_remeasure, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
   tg_remeasure->SetOn(false);
   tg_remeasure->Connect("Clicked()","Rtool",this,"SetRemeasure()");
   
   controls2->AddFrame(tg_gOM, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
   
   // Arbitral measure
   TGGroupFrame *tg_ARB = new TGGroupFrame(controls2, "Arbitral Measurement");
   tg_ARB->SetTitlePos(TGGroupFrame::kCenter);
   
   TGVerticalFrame *ARB_v = new TGVerticalFrame(tg_ARB);
   tg_ARB->AddFrame(ARB_v, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,0,0));
   
   TGCheckButton *tg_isShifter = new TGCheckButton(ARB_v, "Shifter mode");
   ARB_v->AddFrame(tg_isShifter, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
   tg_isShifter->SetOn();
   tg_isShifter->Connect("Clicked()","Rtool",this,"SetShifter()");
   
   TGTextButton *tg_ARBmeas = new TGTextButton(ARB_v, "Measure");
   ARB_v->AddFrame(tg_ARBmeas, new TGLayoutHints(kLHintsRight,
                                              0, 0, 0, 2));
   tg_ARBmeas->Connect("Pressed()", "Rtool", this, "Arbitral()");
   
   controls2->AddFrame(tg_ARB, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
   
   // HVline
   TGGroupFrame *tg_HV = new TGGroupFrame(controls2, "HV line");
   tg_HV->SetTitlePos(TGGroupFrame::kCenter);
   
   //TGVerticalFrame *HV_v = new TGVerticalFrame(tg_HV);
   //tg_HV->AddFrame(HV_v, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,0,0));
   
   //NumberFieldReal *HVrange = new NumberFieldReal(HV_v,"Range(0~11)",0,0,11,15,10,TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber,0);
   //HV_v->AddFrame(HVrange, new TGLayoutHints(kLHintsExpandY | kLHintsRight, 0, 0, 2, 2));
   //tg_HVrange = HVrange->GetEntry();
   
   TGHorizontalFrame *HV_h = new TGHorizontalFrame(tg_HV);
   tg_HV->AddFrame(HV_h, new TGLayoutHints(kLHintsExpandX,0,0));
   
   TGTextButton *tg_HVmeasR = new TGTextButton(HV_h, "Right");
   HV_h->AddFrame(tg_HVmeasR, new TGLayoutHints(kLHintsRight,
                                              0, 0, 0, 2));
   tg_HVmeasR->Connect("Pressed()", "Rtool", this, "HVlineRight()");
   
   TGTextButton *tg_HVmeasL = new TGTextButton(HV_h, "Left");
   HV_h->AddFrame(tg_HVmeasL, new TGLayoutHints(kLHintsRight,
                                              0, 0, 0, 2));
   tg_HVmeasL->Connect("Pressed()", "Rtool", this, "HVlineLeft()");
   
   tg_remeasure_HV = new TGCheckButton(HV_h, "remeasure");
   HV_h->AddFrame(tg_remeasure_HV, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
   tg_remeasure_HV->SetOn(false);
   tg_remeasure_HV->Connect("Clicked()","Rtool",this,"SetRemeasure_HV()");
   
   controls2->AddFrame(tg_HV, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
   
   // Calibration
   TGGroupFrame *tg_Calib = new TGGroupFrame(controls2, "Calibration");
   tg_Calib->SetTitlePos(TGGroupFrame::kCenter);
   
   TGVerticalFrame *Calib_v = new TGVerticalFrame(tg_Calib);
   tg_Calib->AddFrame(Calib_v, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,0,0));
   
   TGTextButton *tg_Calibmake = new TGTextButton(Calib_v, "Create calibration file");
   Calib_v->AddFrame(tg_Calibmake, new TGLayoutHints(kLHintsRight,
                                              0, 0, 0, 2));
   tg_Calibmake->Connect("Pressed()", "Rtool", this, "Calib_create()");
   
   TGTextButton *tg_Calibcalib = new TGTextButton(Calib_v, "Calibration");
   Calib_v->AddFrame(tg_Calibcalib, new TGLayoutHints(kLHintsRight,
                                              0, 0, 0, 2));
   tg_Calibcalib->Connect("Pressed()", "Rtool", this, "Calib_calib()");
   
   controls2->AddFrame(tg_Calib, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
   
   // Savefile
   //TGGroupFrame *tg_save = new TGGroupFrame(controls2, "Savefile");
   //tg_save->SetTitlePos(TGGroupFrame::kCenter);
   
   //TGVerticalFrame *save_v = new TGVerticalFrame(tg_save);
   //tg_save->AddFrame(save_v, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,0,0));
   
   TGTextButton *tg_save = new TGTextButton(controls2, "&Save file");
   //save_v->AddFrame(tg_saveB, new TGLayoutHints(kLHintsRight,
                        //                      0, 0, 0, 2));
   tg_save->Connect("Pressed()", "Rtool", this, "Savefile()");
   
   controls2->AddFrame(tg_save, new TGLayoutHints(kLHintsRight, 0, 0, 2, 2));

   // quit
   TGTextButton *tg_quit = new TGTextButton(controls, "&Exit");
   controls->AddFrame(tg_quit, new TGLayoutHints(kLHintsBottom | kLHintsExpandX,
                                              0, 0, 0, 2));
   tg_quit->Connect("Pressed()", "Rtool", this, "CloseWindow()");

   // pause
   tg_pause = new TGTextButton(controls, "Pause");
   controls->AddFrame(tg_pause, new TGLayoutHints(kLHintsBottom | kLHintsExpandX,
                                              0, 0, 0, 5));
   tg_pause->Connect("Pressed()", "Rtool", this, "Pause()");

  /////////////////////////////////////////////
  
 
  canv->Divide(1,3); 


  time_prev=0;
  time_rate=0;
  n_rate=0;
  gettimeofday(&time_start,NULL);
  char text_time[BUFFSIZE];
  strftime(text_time,20,"%y/%m/%d %T",localtime(&time_start.tv_sec));
  tg_startDate->SetText(text_time);

  //gStyle.SetTimeOffset(0)
  gStyle->SetTimeOffset(time_start.tv_sec);

  thread = new TThread("Update",(void(*)(void*))&Update, (void*)this);
  TThread::SetCancelDeferred();
  TThread::SetCancelOn();
  timerOn=true;
  thread->Run();

  timerPanel = new TTimer(100);//ms
  timerPanel->Connect("Timeout()","Rtool",this,"UpdatePanel()");
  timerPanel->TurnOn();

  timerCanvas = new TTimer(1000);//ms
  timerCanvas->Connect("Timeout()","Rtool",this,"UpdateCanvas()");
  timerCanvas->TurnOn();
  timerCanvasOn=true;

  MapSubwindows();
  Resize();


  //SetWMSizeHints(GetDefaultWidth(), GetDefaultHeight(), 1000, 1000, 0 ,0);
  SetWindowName("Rtool Monitor");
  MapWindow();

}


TThread* Rtool::thread=0;
TCanvas* Rtool::canv=0;
TCanvas* Rtool::canv_Dist=0;
TCanvas* Rtool::canv_preDist=0;
TCanvas* Rtool::canv_HVline=0;
TCanvas* Rtool::canv_Calib=0;

TGTextEntry* Rtool::tg_startDate;
TGTextEntry* Rtool::tg_currentDate;
TGTextEntry* Rtool::tg_elapse;
TGTextEntry* Rtool::tg_counter;
TGTextEntry* Rtool::tg_rate;
TGTextEntry* Rtool::tg_fileType;

TGStatusBar* Rtool::tg_sb;
TGTab* Rtool::tg_tab_contents;

TGLabel* Rtool::tg_lab1;
TGLabel* Rtool::tg_lab2;

TGNumberEntry* Rtool::tg_ndisp;
TGNumberEntry* Rtool::tg_rangeVMin;
TGNumberEntry* Rtool::tg_rangeVMax;
TGNumberEntry* Rtool::tg_relayLeft;
TGNumberEntry* Rtool::tg_relayRight;
TGNumberEntry* Rtool::tg_gOMrange;
TGNumberEntry* Rtool::tg_gOMrange0;
TGNumberEntry* Rtool::tg_HVrange;
TGNumberEntry* Rtool::tg_fileID;
TGNumberEntry* Rtool::tg_pcbID;

TextField* Rtool::write_fileType;
NumberFieldReal* Rtool::write_fileID;
NumberFieldReal* Rtool::write_pcbID;

TGComboBox* Rtool::fCombo;

bool Rtool::timerOn;

double Rtool::value;
int Rtool::unit;
int Rtool::state;
int Rtool::attr;
std::string Rtool::str_value;
std::string Rtool::str_unit;
std::string Rtool::str_state;
std::string Rtool::str_attr;

struct timeval Rtool::time_start;

long Rtool::time;
long Rtool::time_diff;
struct timeval Rtool::event_time;

long Rtool::time_prev;
long Rtool::time_rate;
long Rtool::n_rate;

long Rtool::n;
long Rtool::n_v;
double Rtool::range_v[2];

double Rtool::x_v[nmax/fv];
double Rtool::x_frame_v[2];
double Rtool::y_v[nmax/fv];
double Rtool::y_frame_v[2];

ClassImp(Rtool)

ClassImp(TextField)
ClassImp(NumberFieldInt)
ClassImp(NumberFieldReal)

int main(int argc, char** argv) {  

  ptree ini_reader;
  read_ini("config.ini",ini_reader);
  read_config(ini_reader);

  strcpy(device, device_P.c_str());
  strcpy(device_relay, device_relay_P.c_str());

  TApplication tApp("tApplication", &argc, argv);

  relay = new Relay(device_relay);
  monitor = new Rtool();

  tApp.Run();

  delete monitor;

  return 0;  
}  

