//Comparison
class r_compare {

public:
	r_compare(std::string r_filename)
		:	ref_filename(r_filename)
	{
	}
	double mean_ratio(double mean);
	double rms_ratio(double rms);
	void map_ratio();
	void sheetUpdate();
	std::string ref_filename;
	//TString root_filename_map = "../input/kobe_results/" + pcbTofoilList[ref_filename] + "_ohm.root";//need to change the path
	TString root_filename_map = "../input/kobe_results/" + ref_filename + "_ohm.root";//need to change the path
	TFile *f_map;
private:
	std::string r_filePath="total_quality.dat";
	std::string r_newfilePath="total_quality_new.dat";
	std::string idList = "FoilID,Total_Grade,printingDay,toKobeday,QCday,toCERNday,R_grade,R_mean[Mohm/sq],R_rms,GoodRatio[%],Max,Min,ratio_mu[CH/JP],ratio_rms[CH/JP],width_grade,width_mean[um],width_rms,pitch_grade,pitch_mean[um],pitch_rms,eta[mm]";
};

void r_compare::map_ratio(){
	FileStat_t info;
	TFile *f_check = new TFile(root_filename_map);
	TH1D *h_check = (TH1D*)f_check->Get("Ohmdist");
	if( !gSystem->GetPathInfo(root_filename_map, info) && h_check){
		delete preh1;
		delete preh2;
		f_map = new TFile(root_filename_map);
		if( f_map ){ 
			preh1 = (TH1D*)f_map->Get("Ohmdist");
			preh1->SetName("preOhmdist");
			preh1->SetTitle("preOhmdist");
			preh2 = (TH2D*)f_map->Get("OhmMap");
			preh2->SetName("preOhmMap");
			preh2->SetTitle("preOhmMap");
		}else{
      			preh1 = new TH1D("preOhmdist","preOhmdist",30, 0.0, 3.0);
			preh2 = new TH2D("preOhmMap","preOhmMap",3+currentRange,0,3+currentRange,10,0,10);
		}
	}else{
		std::cout << "No comparison file exists (or corrupted)." << std::endl;
	}
}

void r_compare::sheetUpdate(){
	std::ofstream r_ofs(r_filePath);
	std::ifstream r_ifs(r_newfilePath);
	if(!r_ofs) std::cout << "old dataSheet does not exist !!" << std::endl;
	if(!r_ifs) std::cout << "New dataSheet does not exist !!" << std::endl;	

	if( r_ifs ){
		std::string r_str;
		while(getline(r_ifs,r_str)){
			std::string r_token;
			std::istringstream r_stream(r_str);
			std::string r_table[22];
			int r_n=0;
			while(getline(r_stream,r_token,',')){
				r_table[r_n] = r_token;
				r_n++;
			}	
			for(int i=0; i<r_n; i++){
				if(i!=r_n-1) r_ofs << r_table[i] << ",";
				else r_ofs << r_table[i] << std::endl;
			}
		}
	}
};

double r_compare::mean_ratio(double mean){
	double ratio;
	std::ifstream r_ifs(r_filePath);	
	int n_if=0;
	if(!r_ifs){
		//std::cout << "Reference dataSheet does not exist !!" << std::endl;
		ratio = -1.0;
	}else{
		std::ofstream ofs(r_newfilePath);
	
		std::string r_str;
		while(getline(r_ifs,r_str)){
			std::string r_token;
			std::istringstream r_stream(r_str);
			std::string r_table[22];
			int r_n=0;
			while(getline(r_stream,r_token,',')){
				r_table[r_n] = r_token;
				r_n++;
			}	
			if(ref_filename.compare(r_table[0])==0){ //[1]for dataSheet.csv
				n_if++;
				ratio = int(mean/stof(r_table[7])*100)*0.01; // [8] for dataSheet.csv
				r_table[12] = std::to_string(ratio); // [13] for dataSheet.csv
			}
			for(int i=0; i<r_n; i++){
				if(i!=r_n-1) ofs << r_table[i] << ",";
				else ofs << r_table[i] << std::endl;
			}
		}
		
	}
	if(n_if==0){
		//std::cout << "Reference data for '" << ref_filename << "' does not exist!!" << std::endl;
		ratio = -1;
	}
	FileStat_t info;
	if( !gSystem->GetPathInfo(root_filename_map, info) ){
		double mean_k = preh1->GetMean();
		ratio = int(mean/mean_k*100)*0.01; // [8] for dataSheet.csv
	}else{
		ratio = -1;
	}
	return ratio;
};

double r_compare::rms_ratio(double rms){
	double ratio;
	std::ifstream r_ifs(r_filePath);
	int n_if=0;
	if(!r_ifs){
		//std::cout << "Reference dataSheet does not exist !!" << std::endl;
		ratio = -1.0;
	}else{
		std::ofstream ofs(r_newfilePath);
	
		std::string r_str;
		while(getline(r_ifs,r_str)){
			std::string r_token;
			std::istringstream r_stream(r_str);
			std::string r_table[22];
			int r_n=0;
			while(getline(r_stream,r_token,',')){
				r_table[r_n] = r_token;
				r_n++;
			}	
			if(ref_filename.compare(r_table[0])==0){ //[1] for dataSheet.csv
				n_if++;
				ratio = int(rms/stof(r_table[8])*100)*0.01; // [9] for dataSheet.csv
				r_table[13] = std::to_string(ratio); // [14] for dataSheet.csv
			}
			for(int i=0; i<r_n; i++){
				if(i!=r_n-1) ofs << r_table[i] << ",";
				else ofs << r_table[i] << std::endl;
			}
		}
	}
	if(n_if==0){
		//std::cout << "Reference data for '" << ref_filename << "' does not exist!!" << std::endl;
		ratio = -1;
	}
	FileStat_t info;
	if( !gSystem->GetPathInfo(root_filename_map, info) ){
		double rms_k = preh1->GetRMS();
		ratio = int(rms/rms_k*100)*0.01; // [8] for dataSheet.csv
	}else{
		ratio = -1;
	}
	return ratio;
}

