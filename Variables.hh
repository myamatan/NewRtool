// Setting 
std::string device_P;//fluke 
std::string device_relay_P; 
char device[256];//fluke 
char device_relay[256]; 
bool auto_range;
TString initial_ID;
     
#define BUFFSIZE 256 

//const TString QC_loc = "CERN"; // "JAPAN" or "CERN"
TString QC_loc = "JAPAN"; // "JAPAN" or "CERN"

bool ifOverWrite = false;
bool isRemeasure = false;
bool isRemeasure_HV = false;
bool isShifter = true;

const double ver = 1000601;
bool checkButtonAct = false;

double R[5] = {0.0};
double R2[5] = {0.0};
std::vector<double> Rval;

std::vector<std::vector<double> > vec_vec;
std::vector<std::vector<double> > vec_vec_HV;
std::vector<std::vector<double> > vec_vec_Ca;
std::vector<double> vec;
std::vector<double> vec_HV;
std::vector<double> vec_Ca;
  
std::vector<int> pos_v;
std::vector<std::vector<int> > pos;

int lineCheck = 0;

double tempR = 0.0;
double tempRN = 0.0;

double checkRN = 0.0;

double prlength = 5.0;
double prwidth = 2.0;
double sneak_hosei = 1.56; //error = 0.02
double sneak_hosei10 = 1.16;//error = 0.03
double sneak_hosei15 = 1.23;//error = 0.05
double fact = 1.0;

double fixR = 0.0;
double STD = 0.0;
int countLimit = 5;//80
int probeRange = 10;
int probeRange_HV = 10;
int currentRange = 0;
int currentRange_HV = 0;
int scanrange0 = 0;

Double_t daytime, version;//for data taking infomation
bool save_check = true;
bool meas_check = false;

double target_R = 0.85;
double criteria_SF_u = 3.;
double criteria_SF_l = 3.;

//Prior parameter
const double mu0 = 0.4e+6;//0.4e+6[Mohm/sq]
const double sd0 = 0.16e+6;//0.16e+6[Mohm/sq]
//const double mu0 = 24.0;
//const double sd = 28.0;
double mu = mu0;
double muHV = mu0;
double muCa = mu0;
double sd = sd0;
//double sdHV = sd0;
//double sdCa = sd0;

//for file name
TString file;
char c[8] = ".root";
TString calibrationfile = "../input/calibration.root";
std::string comparisonfile;

std::vector<TString> typeList = {
	"SE1","SE2","SE3","SE4","SE5","SE6","SE7","SE8",
	"SS1","SS2","SS3","SS4","SS5","SS6","SS7","SS8",
	"LE1","LE2","LE3","LE4","LE5","LE6","LE7","LE8",
	"LS1","LS2","LS3","LS4","LS5","LS6","LS7","LS8"
};

// for Scan range refer 
int scanCount = 0;

std::map<TString, std::vector<std::vector<int>> > positionList = {
	{"LE1", {{1,4}, {6,9}} },
	{"LE2", {{4,10},{1,7}} },
	{"LE3", {{2,10},{1,9}} },
	{"LE4", {{1,10},{1,10}} },
	{"LE5", {{1,10},{1,4},{6,9},{1,10}} },
	{"LE6", {{1,10},{3,10},{1,8},{1,10}} },
	{"LE7", {{1,10},{3,10},{1,8},{1,10}} },
	{"LE8", {{1,10},{2,10},{1,9},{1,10}} },

	{"SE1", {{2,4},{6,8}} },
	{"SE2", {{1,4},{6,9}} },
	{"SE3", {{4,10},{1,7}} },
	{"SE4", {{3,10},{1,8}} },
	{"SE5", {{2,10},{1,9}} },//2,10 1,9
	{"SE6", {{1,10},{4,4},{6,6},{1,10}} },
	{"SE7", {{1,10},{3,4},{6,7},{1,10}} },
	{"SE8", {{1,10},{1,4},{6,9},{1,10}} },
	
	{"LS1", {{1,4}, {6,9}} },
	{"LS2", {{4,10},{1,7}} },
	{"LS3", {{2,10},{1,9}} },
	{"LS4", {{1,10},{1,10}} },
	{"LS5", {{1,10},{1,4},{6,9},{1,10}} },
	{"LS6", {{1,10},{3,10},{1,8},{1,10}} },
	{"LS7", {{1,10},{3,10},{1,8},{1,10}} },
	{"LS8", {{1,10},{2,10},{1,9},{1,10}} },

	{"SS1", {{2,4},{6,8}} },
	{"SS2", {{1,4},{6,9}} },
	{"SS3", {{4,10},{1,7}} },
	{"SS4", {{3,10},{1,8}} },
	{"SS5", {{2,10},{1,9}} },
	{"SS6", {{1,10},{3,4},{6,7},{1,10}} },
	{"SS7", {{1,10},{2,4},{6,8},{1,10}} },
	{"SS8", {{1,10},{1,4},{6,9},{1,10}} }
};

//New file, hisogram, graph..
TFile* f=0;
TFile* fca=0;
TFile* fca0=0;
TTree* tr=0;
TTree* tr_info=0;
TH1D* h1=0;
TH2D* h2=0;
TH1D* preh1=0;
TH2D* preh2=0;
TH2D* h2HV=0;
TH2D* h2Ca=0;
TH2D* hCa0=0;
TGraphErrors* tg1=0;
TGraphErrors* tg2=0;

TH1D* hs=0;
