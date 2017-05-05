
#include <time.h>
#include <iostream>
#include <string>

#include <sstream>

#include <TApplication.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TText.h>
#include <TString.h>

#include <TGClient.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TG3DLine.h>
#include <TGFrame.h>
#include <TGLayout.h>
#include <TGWindow.h>
#include <TGStatusBar.h>
#include <TGTab.h>
#include <TGComboBox.h>

#include <TThread.h>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

// Fluke moniter and GUI
struct timeval;      

class TextField : public TGHorizontalFrame {

protected:
   TGTextEntry *fEntry;

public:
   TextField(const TGWindow *p, const char *name, int id=-1, const char* text="", int gap=10) : TGHorizontalFrame(p)
   {
      TGLabel *label = new TGLabel(this, name);
      AddFrame(label, new TGLayoutHints(kLHintsLeft, 2));
      fEntry = new TGTextEntry(this, text, id);
      fEntry->Resize(110,fEntry->GetDefaultHeight());
      AddFrame(fEntry, new TGLayoutHints(kLHintsLeft, gap));
   }
   TGTextEntry *GetEntry() const { return fEntry; }

   ClassDef(TextField, 0)
};

class NumberFieldInt : public TGHorizontalFrame {

protected:
   TGNumberEntry *fEntry;

public:
   NumberFieldInt(const TGWindow *p, const char *name, int id=-1, int val=0, int mval=600, int digits=7, int gap=10) : TGHorizontalFrame(p)
   {
      TGLabel *label = new TGLabel(this, name);
      AddFrame(label, new TGLayoutHints(kLHintsLeft, 2));
      fEntry = new TGNumberEntry(this, val, digits, id, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, mval);
      AddFrame(fEntry, new TGLayoutHints(kLHintsLeft, gap));
   }
   TGNumberEntry *GetEntry() const { return fEntry; }

   ClassDef(NumberFieldInt, 0)
};

class NumberFieldReal : public TGHorizontalFrame {

protected:
   TGNumberEntry *fEntry;

public:
   NumberFieldReal(const TGWindow *p, const char *name, int id=-1, double val=0, double maxval=600, int digits=7, int gap=10, TGNumberFormat::EStyle format=TGNumberFormat::kNESRealOne, TGNumberFormat::EAttribute attr=TGNumberFormat::kNEANonNegative, double minval=0.) : TGHorizontalFrame(p)
   {
      TGLabel *label = new TGLabel(this, name);
      AddFrame(label, new TGLayoutHints(kLHintsLeft, 2));
      fEntry = new TGNumberEntry(this, val, digits, id, format, attr, TGNumberFormat::kNELLimitMinMax, minval, maxval);
      AddFrame(fEntry, new TGLayoutHints(kLHintsLeft, gap));
   }
   TGNumberEntry *GetEntry() const { return fEntry; }

   ClassDef(NumberFieldReal, 0)
};


class Rtool: public TGMainFrame {
private:  
  static const int nmax=2*3600*24*35;//~30days?
  static const int fv=10;//prepare array assuming this skim factor


  static TCanvas* canv;
  static TCanvas* canv_Dist;
  static TCanvas* canv_preDist;
  static TCanvas* canv_HVline;
  static TCanvas* canv_Calib;
  static TThread* thread;

  TGCheckButton* tg_update;
  TGCheckButton* tg_date;
  TGCheckButton* tg_overwrite;
  TGCheckButton* tg_remeasure;
  TGCheckButton* tg_remeasure_HV;
  TGTextButton* tg_pause;

  static TGNumberEntry* tg_ndisp;
  static TGNumberEntry* tg_rangeVMin;
  static TGNumberEntry* tg_rangeVMax;
  static TGNumberEntry* tg_relayLeft;
  static TGNumberEntry* tg_relayRight;
  static TGNumberEntry* tg_gOMrange;
  static TGNumberEntry* tg_gOMrange0;
  static TGNumberEntry* tg_HVrange;
  static TGNumberEntry* tg_fileID;
  static TGNumberEntry* tg_pcbID;
  static TGTextEntry* tg_startDate;
  static TGTextEntry* tg_currentDate;
  static TGTextEntry* tg_elapse;
  static TGTextEntry* tg_counter;
  static TGTextEntry* tg_rate;
  static TGTextEntry* tg_fileType;
  static TGStatusBar* tg_sb;
  static TGTab* tg_tab_contents;
  

  static TGLabel* tg_lab1;
  static TGLabel* tg_lab2;

  TTimer* timerPanel;
  TTimer* timerCanvas;

  static TextField* write_fileType;
  static NumberFieldReal* write_fileID;
  static NumberFieldReal* write_pcbID;

  static TGComboBox* fCombo;

  static bool timerOn;
  bool timerCanvasOn;
  
  static int unit;
  static int state;
  static int attr;
  static double value;
  static std::string str_value;
  static std::string str_unit;
  static std::string str_state;
  static std::string str_attr;

  static struct timeval time_start;

  static long time;
  static long time_diff;
  static struct timeval event_time;

  static long time_prev;
  static long time_rate;
  static long n_rate;

  long ndisp;

  static long n;
  static long n_v;
  static double range_v[2];


  TGraph* g_v;
  TGraph* g_frame_v;

  static double x_v[nmax/fv]; 
  static double x_frame_v[2]; 
  static double y_v[nmax/fv];
  static double y_frame_v[2];

public:
  Rtool();
  virtual ~Rtool();
  void Finalize();
  void CloseWindow();
  void Pause();
  void PauseCanvas();
  void PrintCanvas(std::string name);
  static void Update();
  void UpdatePanel();
  void UpdateCanvas();
  void ChangeNDisp();
  void ChangeRangeV();
  void BackRange();
  void NextRange();

  void relaySwitch();
  void SetNewfile();
  void SetOverWrite();
  void SetRemeasure();
  void SetRemeasure_HV();
  void SetShifter();
  void getOhmMap();
  void Arbitral();
  void HVlineLeft();
  void HVlineRight();
  void Calib_create();
  void Calib_calib();
  void Savefile();

  ClassDef(Rtool,0)

};


