void Rtool::HVlineLeft(){
  
	int scanrangeS = 0;
      	int scanrangeE = 0;
	int flip_number = 0;
	if( QC_loc == "JAPAN" ) flip_number =  10;
	else flip_number = 0;
      	if( !isRemeasure_HV ){ 
      		scanrangeS = 0 + flip_number;
      		scanrangeE = 1 + flip_number;
      		probeRange_HV = scanrangeE-scanrangeS;
      	}else if( isRemeasure_HV && !(currentRange_HV == 0) ){
      		for(int s=0; s<probeRange_HV; s++){
      			vec_vec_HV.pop_back();
      		}
      		currentRange_HV -= probeRange_HV;
      		scanrangeS = 0 + flip_number;
      		scanrangeE = 1 + flip_number;
      		probeRange_HV = scanrangeE-scanrangeS;
	}else{
      		scanrangeS = 0;
      		scanrangeE = 0;
      		probeRange_HV = scanrangeE-scanrangeS;
	}
	
	if( isRemeasure_HV ){
		tg_remeasure_HV->SetOn(false);
		SetRemeasure_HV();
	}
      
	usleep(200000);
      
	std::vector<int> pos_v;
      	std::vector<std::vector<int> > pos;
      
	int count = 0;
      	int loopcount = 0;
      	if( currentRange_HV == 0 ) muHV = mu0*20;
      	//for(int i=scanrangeS; i<scanrangeE; i++){//i=0;i<11;i++
	for(int i=0; i<scanrangeE-scanrangeS; i++){//i=0;i<11;i++
		for(int j=1; j<10; j++){//j=0; j<10; j++
			int A = (i+scanrangeS)*9 + j;
			int B = 100;
			for(int s=0; s<5; s++){
				R[s] = 0.0;
				R2[s] = 0.0;
			}
			R[4] = 1.0e+10;
			relay->change(A, B);//(Left, Right) //Right88 is broken in Relay Box (Japan)
			checkButtonAct = true;
			while(checkRN < 1 && count < countLimit){//500000
				usleep(100*1000);
				checkRN = tempRN;
				count++;
				std::cout << "[" << A << ", " << B << "] : " << count << "/" << countLimit << std::endl;
				std::cout << value << " [Ohm/sq]"<< std::endl;
			}
			fixR = value;
			double ul = muHV+100*sd;
			double dl = muHV-100*sd;
			if( dl < 1.0 ) dl = 1.0;
			if( fixR > dl && fixR < ul ){
				vec.push_back(fixR/1e+6);
				muHV = (muHV+fixR)/2.;
				loopcount = 0;
			}else if( loopcount < 2 && j > 1 ){
				j--;
			}else if( loopcount < 2 && j == 1 && i > 0){
				i--;
				j = 9;
			}else if( loopcount < 2 && j == 1 && i == 0){
				j--;
			}else if( loopcount >= 2 && fixR > 1.0e+20){ 
				vec.push_back(0.0);
				pos_v.push_back(A);
				pos_v.push_back(B);
				pos.push_back(pos_v);
				pos_v.erase( pos_v.begin(), pos_v.end() );
				loopcount = 0;
			}else if( loopcount >= 2 && fixR < 1.0e+20 ){
				vec.push_back(fixR/1e+6);
				pos_v.push_back(A);
				pos_v.push_back(B);
				pos.push_back(pos_v);
				pos_v.erase( pos_v.begin(), pos_v.end() );
				muHV = (muHV+fixR)/2.;
				loopcount = 0;
			}
			loopcount++;
			checkButtonAct = false;
			fixR = 0.0;
			tempR = 0.0;
			tempRN = 0.0;
			count = 0;
			checkRN = 0.0;	
		}
	
		vec_vec_HV.push_back(vec);
		if( vec_vec_HV.size() != i+currentRange_HV+1 && vec_vec_HV.size() > 0) vec_vec_HV.pop_back();/////okyustoti
		//tr->Fill();
		vec.erase( vec.begin(), vec.end() );
	}
      
	delete h2HV;
      	h2HV = new TH2D("OhmMapfromHV","OhmMapfromHV",probeRange_HV+currentRange_HV,0,(probeRange_HV+currentRange_HV)*5,9,0,9*5);
	for(int i=0; i<probeRange_HV+currentRange_HV; i++){//i=0; i<11+currentRange_HV; i++
      		for(int j=1; j<10; j++){//j=0; j<9; j++
      			h2HV->SetBinContent(i+1, 10-j, vec_vec_HV[i][j-1]);
      		}
	}
      
	currentRange_HV += probeRange_HV;//+11
      
	canv_HVline->cd();
      	h2HV->SetStats(0);
      	gStyle->SetPaintTextFormat("3.2f");
      	h2HV->GetZaxis()->SetRangeUser(0,70.0);
      	h2HV->Draw("colz text45");
      	h2HV->SetMarkerSize(2);
      	canv->cd();
      
	std::cout << "[ Error or non-conductive area ]" << std::endl;	
      	for(int i=0; i<pos.size(); i++){
      		double er_value = vec_vec_HV[currentRange_HV-probeRange_HV+(pos[i][0]-1)/9-scanrangeS][pos[i][0]-(pos[i][0]-1)/9*9-1];
      		std::cout << "[" << pos[i][0] << ", " << pos[i][1] << "] : " << er_value << "[Mohm/sq]" << std::endl;
       	}
      	return;
}

void Rtool::HVlineRight(){
  
	int scanrangeS = 0;
      	int scanrangeE = 0;
	int flip_number = 0;
	if( QC_loc == "JAPAN" ) flip_number =  -10;
	else flip_number = 0;
      	if( !isRemeasure_HV ){ 
      		scanrangeS = 10 + flip_number;
      		scanrangeE = 11 + flip_number;
      		probeRange_HV = scanrangeE-scanrangeS;
      	}else if( isRemeasure_HV && !(currentRange_HV == 0) ){
      		for(int s=0; s<probeRange_HV; s++){
      			vec_vec_HV.pop_back();
      		}
      		currentRange_HV -= probeRange_HV;
      		scanrangeS = 10 + flip_number;
      		scanrangeE = 11 + flip_number;
      		probeRange_HV = scanrangeE-scanrangeS;
	}else{
      		scanrangeS = 0;
      		scanrangeE = 0;
      		probeRange_HV = scanrangeE-scanrangeS;
	}
	
	if( isRemeasure_HV ){
		tg_remeasure_HV->SetOn(false);
		SetRemeasure_HV();
	}
      
	usleep(200000);
      
	std::vector<int> pos_v;
      	std::vector<std::vector<int> > pos;
      
	int count = 0;
      	int loopcount = 0;
      	if( currentRange_HV == 0 ) muHV = mu0*20;
      	//for(int i=scanrangeS; i<scanrangeE; i++){//i=0;i<11;i++
	for(int i=0; i<scanrangeE-scanrangeS; i++){//i=0;i<11;i++
		for(int j=1; j<10; j++){//j=0; j<10; j++
			int A = (i+scanrangeS)*9 + j;
			int B = 100;
			for(int s=0; s<5; s++){
				R[s] = 0.0;
				R2[s] = 0.0;
			}
			R[4] = 1.0e+10;
			relay->change(A, B);//(Left, Right) //Right88 is broken in Relay Box (Japan)
			checkButtonAct = true;
			while(checkRN < 1 && count < countLimit){//500000
				usleep(100*1000);
				checkRN = tempRN;
				count++;
				std::cout << "[" << A << ", " << B << "] : " << count << "/" << countLimit << std::endl;
				std::cout << value << " [Ohm/sq]"<< std::endl;
			}
			fixR = value;
			double ul = muHV+100*sd;
			double dl = muHV-100*sd;
			if( dl < 1.0 ) dl = 1.0;
			if( fixR > dl && fixR < ul ){
				vec.push_back(fixR/1e+6);
				muHV = (muHV+fixR)/2.;
				loopcount = 0;
			}else if( loopcount < 2 && j > 1 ){
				j--;
			}else if( loopcount < 2 && j == 1 && i > 0){
				i--;
				j = 9;
			}else if( loopcount < 2 && j == 1 && i == 0){
				j--;
			}else if( loopcount >= 2 && fixR > 1.0e+20){ 
				vec.push_back(0.0);
				pos_v.push_back(A);
				pos_v.push_back(B);
				pos.push_back(pos_v);
				pos_v.erase( pos_v.begin(), pos_v.end() );
				loopcount = 0;
			}else if( loopcount >= 2 && fixR < 1.0e+20 ){
				vec.push_back(fixR/1e+6);
				pos_v.push_back(A);
				pos_v.push_back(B);
				pos.push_back(pos_v);
				pos_v.erase( pos_v.begin(), pos_v.end() );
				muHV = (muHV+fixR)/2.;
				loopcount = 0;
			}
			loopcount++;
			checkButtonAct = false;
			fixR = 0.0;
			tempR = 0.0;
			tempRN = 0.0;
			count = 0;
			checkRN = 0.0;	
		}
	
		vec_vec_HV.push_back(vec);
		if( vec_vec_HV.size() != i+currentRange_HV+1 && vec_vec_HV.size() > 0) vec_vec_HV.pop_back();/////okyustoti
		//tr->Fill();
		vec.erase( vec.begin(), vec.end() );
	}
      
	delete h2HV;
      	h2HV = new TH2D("OhmMapfromHV","OhmMapfromHV",probeRange_HV+currentRange_HV,0,(probeRange_HV+currentRange_HV)*5,9,0,9*5);
	for(int i=0; i<probeRange_HV+currentRange_HV; i++){//i=0; i<11+currentRange_HV; i++
      		for(int j=1; j<10; j++){//j=0; j<9; j++
      			h2HV->SetBinContent(i+1, 10-j, vec_vec_HV[i][j-1]);
      		}
	}
      
	currentRange_HV += probeRange_HV;//+11
      
	canv_HVline->cd();
      	h2HV->SetStats(0);
      	gStyle->SetPaintTextFormat("3.2f");
      	h2HV->GetZaxis()->SetRangeUser(0,70.0);
      	h2HV->Draw("colz text45");
      	h2HV->SetMarkerSize(2);
      	canv->cd();
      
	std::cout << "[ Error or non-conductive area ]" << std::endl;	
      	for(int i=0; i<pos.size(); i++){
      		double er_value = vec_vec_HV[currentRange_HV-probeRange_HV+(pos[i][0]-1)/9-scanrangeS][pos[i][0]-(pos[i][0]-1)/9*9-1];
      		std::cout << "[" << pos[i][0] << ", " << pos[i][1] << "] : " << er_value << "[Mohm/sq]" << std::endl;
       	}
      	return;
}

void Rtool::Calib_calib(){

	relay->change(1,10);
	int count = 0;
	int loopcount = 0;
	muCa = mu0;
	std::vector<double> ylist1;
	std::vector<double> ylist2;
	std::vector<double> ylist3;
	std::vector<double> ey;
	std::vector<double> xlist;
	std::vector<double> ex;
	int check_ca = 0;
	for(int i=0; i<10; i++){
		for(int j=1; j<10; j++){
			int A = i*9 + j;
			int B = A + 9;
			for(int s=0; s<5; s++){
				R[s] = 0.0;
				R2[s] = 0.0;
			}
			R[4] = 1.0e+10;
			relay->change(A, B);//(Left, Right)
			checkButtonAct = 1.0;
			while(checkRN < 1 && count < countLimit){
				usleep(100*1000);
				checkRN = tempRN;
				count++;
				std::cout << "[" << A << ", " << B << "] : " << count << "/" << countLimit << std::endl;
				std::cout << value * prwidth / prlength *sneak_hosei << " [Ohm/sq]"<< std::endl;
			}
			fixR = value * prwidth / prlength * sneak_hosei;
			double ul = muCa+3*sd;
			double dl = muCa-3*sd;
			if( dl < 1.0 ) dl = 1.0;
			if( fixR > dl && fixR < ul ){
			     	vec.push_back(fixR/1e+6);
				muCa = (muCa+fixR)/2.;
				loopcount = 0;
			}else if( loopcount < 2 && j > 1 ){
				j--;
			}else if( loopcount < 2 && j == 1 && i > 0){
				i--;
				j = 9;
			}else if( loopcount < 2 && j == 1 && i == 0 ){
				j--;
			}else if( loopcount >= 2 && fixR > 1.0e+20){ 
		    		vec.push_back(0.0);
				loopcount = 0;
			}else if( loopcount >= 2 && fixR < 1.0e+20 ){
				vec.push_back(fixR/1e+6);
				muCa = (muCa+fixR)/2.;
				loopcount = 0;
			}
			loopcount++;
			checkButtonAct = 0.0;
			fixR = 0.0;
			tempR = 0.0;
			tempRN = 0.0;
			count = 0;
			checkRN = 0.0;	
		}
		vec_vec_Ca.push_back(vec);
		if( vec_vec_Ca.size() != i+1 && vec_vec_Ca.size() > 0) vec_vec_Ca.pop_back();/////okyustoti
		vec.erase( vec.begin(), vec.end() );
	}
	
	delete fca0;
	//delete hCa0;
	fca0 = new TFile(calibrationfile);
	hCa0 = (TH2D*)fca0->Get("calibMap");
	double chi2sum = 0.0;
	int df = 0;
	double ca_ratio;
	for(int i=0; i<10; i++){
		for(int j=1; j<10; j++){
			double value1 = hCa0->GetBinContent(i+1, 10-j);
			double value2 = vec_vec_Ca[i][j-1];
			if( value1 > 0.001 && value2 > 0.001 ){
				chi2sum += pow((value2-value1), 2) / value1 * 1e+1;
				df++;
				ca_ratio = value2 / value1;
				if( ca_ratio < 0.8 || ca_ratio > 1.2 ) check_ca = 1;
			}
			xlist.push_back((i*9+j));
			ylist1.push_back(value1);
			ylist2.push_back(value2);
			ylist3.push_back(ca_ratio);
			ex.push_back(0.0);
			ey.push_back(0.0);
		}
	}
	double quantile = TMath::ChisquareQuantile(0.95, df-1);
	
	std::cout << "chiSquare = " << chi2sum << std::endl;
	std::cout << "chisquared_cdf(alpha = 5 %, df-1=" << df-1 << ") = " << quantile << std::endl;
	/*
	 * if(chi2sum > quantile){
		std::cout << "Calibration failed..." << std::endl;
	}else{
			std::cout << "Calibration successed..." << std::endl;
		}
		*/
	if( check_ca == 1 ) std::cout << "Calibration failed..." << std::endl;
	else std::cout << "Calibration successed..." << std::endl;

	Double_t* xpointer=&(xlist.begin()[0]);
   	//Double_t* ypointer1=&(ylist1.begin()[0]);
 	//Double_t* ypointer2=&(ylist2.begin()[0]);
	Double_t* ypointer3=&(ylist3.begin()[0]);
	Double_t* xEpointer=&(ex.begin()[0]);
	Double_t* yEpointer=&(ey.begin()[0]);
	delete tg1;
	//delete tg2;
	//tg1 = new TGraphErrors(xlist.size(), xpointer, ypointer1, xEpointer, yEpointer);
	//tg2 = new TGraphErrors(xlist.size(), xpointer, ypointer2, xEpointer, yEpointer);
	tg1 = new TGraphErrors(xlist.size(), xpointer, ypointer3, xEpointer, yEpointer);
		
	canv_Calib->cd();
	tg1->SetMarkerStyle(4);
	tg1->SetMarkerColor(1);
	tg1->SetMarkerSize(0.7);
	tg1->GetXaxis()->SetTitle("probe ID");
	tg1->SetTitle("Ratio [current/reference]");
	tg1->GetYaxis()->SetTitle("ratio [current/reference]");
	tg1->SetMaximum(2.0);
	tg1->SetMinimum(0.0);
	tg1->Draw("AP");
	/*
	tg2->SetMarkerStyle(4);
	tg2->SetMarkerSize(0.6);
	tg2->SetMarkerColor(2);
	tg2->Draw("P");
	*/
	//TLegend *leg = new TLegend(0.95,0.7,0.48,0.9);
	//leg->AddEntry(tg1, "Ratio", "p");
	//leg->AddEntry(tg1, "Reference", "p");
	//leg->AddEntry(tg2, "Current state", "p");
	//leg->SetBorderSize(0);
	//leg->SetFillStyle(0);
	//leg->SetFillColor(0);
	//leg->Draw();
	canv->cd();
	
	vec_vec_Ca.erase( vec_vec_Ca.begin(), vec_vec_Ca.end() );
	return;

}

void Rtool::Calib_create(){
	
	int count = 0;
	int loopcount = 0;
	muCa = mu0;
	std::vector<double> ylist1;
	std::vector<double> ylist2;
	std::vector<double> ylist3;
	std::vector<double> ey;
	std::vector<double> xlist;
	std::vector<double> ex;

	for(int i=0; i<10; i++){
		for(int j=1; j<10; j++){
			int A = i*9 + j;
			int B = A + 9;
			for(int s=0; s<5; s++){
				R[s] = 0.0;
				R2[s] = 0.0;
			}
			R[4] = 1.0e+10;
			relay->change(A, B);//(Left, Right)
			checkButtonAct = 1.0;
			while(checkRN < 1 && count < countLimit){
				usleep(100*1000);
				checkRN = tempRN;
				count++;
				std::cout << "[" << A << ", " << B << "] : " << count << "/" << countLimit << std::endl;
				std::cout << value * prwidth / prlength *sneak_hosei << " [Ohm/sq]"<< std::endl;
			}
			fixR = value * prwidth / prlength * sneak_hosei;
			double ul = muCa+3*sd;
			double dl = muCa-3*sd;
			if( dl < 1.0 ) dl = 1.0;
			if( fixR > dl && fixR < ul ){
			     	vec.push_back(fixR/1e+6);
				muCa = (muCa+fixR)/2.;
				loopcount = 0;
			}else if( loopcount < 2 && j > 1 ){
				j--;
			}else if( loopcount < 2 && j == 1 && i > 0){
				i--;
				j = 9;
			}else if( loopcount < 2 && j == 1 && i == 0 ){
				j--;
			}else if( loopcount >= 2 && fixR > 1.0e+20){ 
		    		vec.push_back(0.0);
				loopcount = 0;
			}else if( loopcount >= 2 && fixR < 1.0e+20 ){
				vec.push_back(fixR/1e+6);
				muCa = (muCa+fixR)/2.;
				loopcount = 0;
			}
			loopcount++;
			checkButtonAct = 0.0;
			fixR = 0.0;
			tempR = 0.0;
			tempRN = 0.0;
			count = 0;
			checkRN = 0.0;	
		}
		vec_vec_Ca.push_back(vec);
		if( vec_vec_Ca.size() != i+1 && vec_vec_Ca.size() > 0) vec_vec_Ca.pop_back();/////okyustoti
		vec.erase( vec.begin(), vec.end() );
	}

	delete h2Ca;
	h2Ca = new TH2D("calibMap","calibMap",10, 0, (10)*5, 9, 0, 9*5);
	for(int i=0; i<10; i++){
		for(int j=1; j<10; j++){
			h2Ca->SetBinContent(i+1, 10-j, vec_vec_Ca[i][j-1]);
		}
	}
	delete fca;
	fca = new TFile(calibrationfile,"recreate");
	fca->cd();
	h2Ca->Write();
	fca->Close();
	std::cout << "Calibration file has been made." << std::endl;

	vec_vec_Ca.erase( vec_vec_Ca.begin(), vec_vec_Ca.end() );
	return;

}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void Rtool::Arbitral(){

	int loop_fig=1;
	int loop_fig3=1;
	int value_handle;
	int check_arbi;
	count_Arbi++;
	prlength = 5;
	if( !isShifter ){

		while( loop_fig == 1 ){
			std::cout << "[ Error or non-conductive area ]" << std::endl;	
			for(int i=0; i<pos.size(); i++){
				double er_value = vec_vec[currentRange-probeRange-scanrange0+(pos[i][0]-1)/9][pos[i][0]-(pos[i][0]-1)/9*9-1];
				std::cout << "[" << pos[i][0] << ", " << pos[i][1] << "] : " << er_value << "[Mohm/sq]" << std::endl;	
			}
			std::cout << "Measurement method ------- ?" << std::endl;
			std::cout << "0 : Just re-measure  " << std::endl;
			std::cout << "1 : Estimation " << std::endl;
			int re_me_type;
			std::cin >> re_me_type;

			std::cout << "Enter First probe ID----" << std::endl;
			int ID1 = 0;
			std::cin >> ID1;
			std::cout << "------ First probe ID = " << ID1 << std::endl;
			std::cout << "Enter Second probe ID----" << std::endl;
			int ID2 = 0;
			std::cin >> ID2;
			std::cout << "------ Second probe ID = " << ID2 << std::endl;
			if( ID1 == ID2 + 9 ){
				int ID1_k = ID1;
				int ID2_k = ID2;
				ID1 = ID2_k;
				ID2 = ID1_k;
			}
			if( re_me_type == 0 ){
				relay->change(ID1, ID2);	
				std::cout << "Current activated probe [left, Right] = [" << ID1 << ", " << ID2 << "]" << std::endl;
				while( loop_fig3 == 1){
					usleep(1000*1000);
					fixR = value * prwidth / prlength * sneak_hosei * fact;
					std::cout << "value = " << fixR/1e+6 << "[Mohm/sq]" << std::endl;
					std::cout << "Fill this value ?" << std::endl;
					std::cout <<	"0 : No" << std::endl;
					std::cout <<	"1 : Yes (OhmMap)" << std::endl;
					std::cout <<	"2 : Re-measure" << std::endl;
					std::cin >> value_handle;
					if( value_handle == 1){
						check_arbi = 1;
						std::cout << "Chekck other probe ?" << std::endl;
						std::cout << "0 : No" << std::endl;
						std::cout << "1 : Yes" << std::endl;
						std::cin >> loop_fig;
						loop_fig3 = 0;
					}else if( value_handle == 0 ){
						check_arbi = 999;
						std::cout << "Chekck other probe ?" << std::endl;
						std::cout << "0 : No" << std::endl;
						std::cout << "1 : Yes" << std::endl;
						std::cin >> loop_fig;
						loop_fig3 = 0;
					}else{
						check_arbi = 999;
						loop_fig = 1;
						loop_fig3 = 1;
					}	
				}
				loop_fig3 = 1;

				if( check_arbi == 1 && ID2 == ID1+9 && ID1 > 0 && ID2 <= (probeRange+scanrange0+1)*9){
					if( fixR > 1e+20 ) fixR = 0.0;
					vec_vec[currentRange-probeRange-scanrange0+(ID1-1)/9][ID1-(ID1-1)/9*9-1] = fixR/1e+6;
					delete h1;
					delete h2;
					h1 = new TH1D("Ohmdist","Ohmdist",60, 0.0, 6.0);
					h2 = new TH2D("OhmMap","OhmMap",currentRange, 0, currentRange*5, 9, 0, 9*5);
					for(int i=0; i<currentRange; i++){//i=0; i<10+currentRange; i++
						for(int j=1; j<10; j++){//j=0; j<10; j++
							h2->SetBinContent(i+1, 10-j, vec_vec[i][j-1]);// /1000 00
							if( vec_vec[i][j-1] > 0.01 ) h1->Fill(vec_vec[i][j-1]);
						}
					}
					canv_Dist->cd(1);
					h1->Draw();
					canv_Dist->cd(2);
					h2->SetStats(0);
					gStyle->SetPaintTextFormat("3.2f");
					h2->GetZaxis()->SetRangeUser(0,3.0);
					h2->Draw("colz text45");
					h2->SetMarkerSize(2);
					canv->cd();
				}
			}else if( re_me_type == 1 ){
		
				while( loop_fig3 == 1){
					double v1,v2,v3,v4;
					double vN = 0.0;
					double vsum = 0.0;
				
					double ul = (h1) ? h1->GetMean()+3*h1->GetRMS():3e+6;
					double dl = (h1) ? h1->GetMean()-5*h1->GetRMS():3e+6;
					if( dl < 0.0 ) dl = 0.0;

					//v1
					if( ID1 > 0 && ID2 <= (probeRange+1)*9 ){ 
						fact = 1.0;
						relay->change(ID1, ID2);	
						usleep(1000*1000);
						std::cout << "Current activated probe [left, Right] = [" << ID1 << ", " << ID2 << "]" << std::endl;
						v1 = value * prwidth / prlength * sneak_hosei * fact;
						std::cout << "v1 = " << v1 << " [Mohm/sq]" << std::endl;
						if( v1 < ul && v1 > dl ){
							vsum += v1;
							vN += 1.0;
						}
					}				
					//v2
					if( ID1 > 0 && ID2 <= probeRange*9 ){ 
						prlength = 10;
						fact = sneak_hosei10;
						relay->change(ID1, ID2+9);	
						usleep(1000*1000);
						std::cout << "Current activated probe [left, Right] = [" << ID1 << ", " << ID2+9 << "]" << std::endl;
						v2 = value * prwidth / prlength * sneak_hosei * fact;
						std::cout << "v2 = " << v2 << " [Mohm/sq]" << std::endl;
						if( v2 < ul && v2 > dl ){
							vsum += v2;
							vN += 1.0;
						}
					}
					//v3
					if( ID1 > 9 && ID2 <= (probeRange+1)*9 ){ 
						prlength = 10;
						fact = sneak_hosei10;
						relay->change(ID1-9, ID2);	
						usleep(1000*1000);
						std::cout << "Current activated probe [left, Right] = [" << ID1-9 << ", " << ID2 << "]" << std::endl;
						v3 = value * prwidth / prlength * sneak_hosei * fact;
						std::cout << "v3 = " << v3 << " [Mohm/sq]" << std::endl;
						if( v3 < ul && v3 > dl ){
							vsum += v3;
							vN += 1.0;
						}
					}
					//v4
					if( ID1 > 9 && ID2 <= probeRange*9 ){ 
						prlength = 15;
						fact = sneak_hosei15;
						relay->change(ID1-9, ID2+9);	
						usleep(1000*1000);
						std::cout << "Current activated probe [left, Right] = [" << ID1-9 << ", " << ID2+9 << "]" << std::endl;
						v4 = value * prwidth / prlength * sneak_hosei * fact;
						std::cout << "v4 = " << v4 << " [Mohm/sq]" << std::endl;
						if( v4 < ul && v4 > dl ){
							vsum += v4;
							vN += 1.0;
						}
					}
					prlength = 5.;
					fact = 1.;
				
					fixR = vsum / vN;
					std::cout << "value = " << fixR/1e+6 << "[Mohm/sq]" << std::endl;
					std::cout << "Fill this value ?" << std::endl;
					std::cout <<	"0 : No" << std::endl;
					std::cout <<	"1 : Yes (OhmMap)" << std::endl;
					std::cout <<	"2 : Re-measure" << std::endl;
					std::cin >> value_handle;
					if( value_handle == 1){
						check_arbi = 1;
						std::cout << "Chekck other probe ?" << std::endl;
						std::cout << "0 : No" << std::endl;
						std::cout << "1 : Yes" << std::endl;
						std::cin >> loop_fig;
						loop_fig3 = 0;
					}else if( value_handle == 0 ){
						check_arbi = 999;
						std::cout << "Chekck other probe ?" << std::endl;
						std::cout << "0 : No" << std::endl;
						std::cout << "1 : Yes" << std::endl;
						std::cin >> loop_fig;
						loop_fig3 = 0;
					}else{
						check_arbi = 999;
						loop_fig = 1;
						loop_fig3 = 1;
					}	
				}
				loop_fig3 = 1;

				if( check_arbi == 1 && ID2 == ID1+9 ){
					if( fixR > 1e+20 ) fixR = 0.0;
					vec_vec[currentRange-probeRange-scanrange0+(ID1-1)/9][ID1-(ID1-1)/9*9-1] = fixR/1e+6;
					delete h1;
					delete h2;
					h1 = new TH1D("Ohmdist","Ohmdist",30, 0.0, 3.0);
					h2 = new TH2D("OhmMap","OhmMap",currentRange, 0, currentRange*5, 9, 0, 9*5);
					for(int i=0; i<currentRange; i++){//i=0; i<10+currentRange; i++
						for(int j=1; j<10; j++){//j=0; j<10; j++
							h2->SetBinContent(i+1, 10-j, vec_vec[i][j-1]);// /1000 00
							if( vec_vec[i][j-1] > 0.01 ) h1->Fill(vec_vec[i][j-1]);
						}
					}
					canv_Dist->cd(1);
					h1->Draw();
					canv_Dist->cd(2);
					h2->SetStats(0);
					gStyle->SetPaintTextFormat("3.2f");
					h2->GetZaxis()->SetRangeUser(0,3.0);
					h2->Draw("colz text45");
					h2->SetMarkerSize(2);
					canv->cd();
				}
			}
		}
		std::cout << "Arbitral measurement done" << std::endl;
	}else{  // Shifter mode 
		currentRange -= probeRange;//+10
		int count = 0;
		int loopcount = 0;
		double v_kari = 0.0;
		// Re-measure points which returned Strange value
		std::vector<int> check_pos;
		double ul;
		double dl;
		for(int i=0; i<pos.size(); i++){
			int A = pos[i][0];
			int B = pos[i][1];
			//double er_value = vec_vec[currentRange-probeRange+(pos[i][0]-1)/9][pos[i][0]-(pos[i][0]-1)/9*9-1];
		        for(int s=0; s<5; s++){
				R[s] = 0.0;
				R2[s] = 0.0;
			}
			R[4] = 1.0e+10;
			relay->change(A, B);//(Left, Right)
			checkButtonAct = true;
			while(checkRN < 1 && count < countLimit){
				usleep(100*1000);
				checkRN = tempRN;
				count++;
				std::cout << "[" << A << ", " << B << "] : " << count << "/" << countLimit << std::endl;
				std::cout << value * prwidth / prlength * sneak_hosei * fact << " [Ohm/sq]"<< std::endl;
			}
			checkButtonAct = false;
			checkRN = 0.0;	
			count = 0;
			fixR = value *  prwidth / prlength * sneak_hosei * fact;
			ul = mu+3*sd;
			dl = mu-3*sd;
			if( dl < 1.0 ) dl = 1.0;
			if( fixR > dl && fixR < ul ){//fixR < 5.0e+6
				vec_vec[currentRange-scanrange0+(pos[i][0]-1)/9][pos[i][0]-(pos[i][0]-1)/9*9-1] = fixR/1e+6;
				mu = (mu+fixR)/2.;
				prlength = 5;
				fact = 1.0;
				loopcount = 0;
				countLimit = 5;
				check_pos.push_back(i);
			}else if( loopcount < 2){
				v_kari = fixR;
				i--;
			}else if( loopcount >= 2 ){
				double v1,v2,v3,v4;
				double vN = 0.0;
				double vsum = 0.0;
				//v1
				if( A > 0 && B <= (probeRange+scanrange0+1)*9 ){ 
					fact = 1.0;
					relay->change(A, B);	
					checkButtonAct = true;
					while(checkRN < 1 && count < countLimit){
						usleep(100*1000);
						checkRN = tempRN;
						count++;
						std::cout << "[" << A << ", " << B << "] : " << count << "/" << countLimit << std::endl;
						std::cout << value * prwidth / prlength * sneak_hosei * fact << " [Ohm/sq]"<< std::endl;
					}
					checkRN = 0.0;	
					count = 0;
					checkButtonAct = false;
					std::cout << "Current activated probe [left, Right] = [" << A << ", " << B << "]" << std::endl;
					v1 = value * prwidth / prlength * sneak_hosei * fact;
					std::cout << "v1 = " << v1 << " [Mohm/sq]" << std::endl;
					if( v1 < ul && v1 > dl ){
						std::cout << "Good value ------------------" << std::endl;
						vsum += v1;
						vN += 1.0;		
					}
				}				
				//v2
			        if( A > 0 && B <= (probeRange+scanrange0)*9 ){ 
					prlength = 10;
					fact = sneak_hosei10;
					relay->change(A, B+9);	
					checkButtonAct = true;
					while(checkRN < 1 && count < countLimit){
						usleep(100*1000);
						checkRN = tempRN;
						count++;
						std::cout << "[" << A << ", " << B << "] : " << count << "/" << countLimit << std::endl;
						std::cout << value * prwidth / prlength * sneak_hosei * fact << " [Ohm/sq]"<< std::endl;
					}
					checkRN = 0.0;	
					count = 0;
					checkButtonAct = false;
					std::cout << "Current activated probe [left, Right] = [" << A << ", " << B+9 << "]" << std::endl;
					v2 = value * prwidth / prlength * sneak_hosei * fact;
					std::cout << "v2 = " << v2 << " [Mohm/sq]" << std::endl;
					if( v2 < ul && v2 > dl ){			 
						std::cout << "Good value ------------------" << std::endl;
						vsum += v2;
						vN += 1.0;
					}
				}
				//v3	
				if( A > 9 && B <= (probeRange+scanrange0+1)*9 ){ 
					prlength = 10;
					fact = sneak_hosei10;
					relay->change(A-9, B);	
					checkButtonAct = true;
					while(checkRN < 1 && count < countLimit){
						usleep(100*1000);
						checkRN = tempRN;
						count++;
						std::cout << "[" << A << ", " << B << "] : " << count << "/" << countLimit << std::endl;
						std::cout << value * prwidth / prlength * sneak_hosei * fact << " [Ohm/sq]"<< std::endl;
					}
					checkRN = 0.0;	
					count = 0;
					checkButtonAct = false;
					std::cout << "Current activated probe [left, Right] = [" << A-9 << ", " << B << "]" << std::endl;
					v3 = value * prwidth / prlength * sneak_hosei * fact;
					std::cout << "v3 = " << v3 << " [Mohm/sq]" << std::endl;
					if( v3 < ul && v3 > dl ){
						std::cout << "Good value ------------------" << std::endl;
						vsum += v3;
						vN += 1.0;
					}
				}
				//v4
				if( A > 9 && B <= (probeRange+scanrange0)*9 ){ 
					prlength = 15;
					fact = sneak_hosei15;
					relay->change(A-9, B+9);	
					checkButtonAct = true;
					while(checkRN < 1 && count < countLimit){
						usleep(100*1000);
						checkRN = tempRN;
						count++;
						std::cout << "[" << A << ", " << B << "] : " << count << "/" << countLimit << std::endl;
						std::cout << value * prwidth / prlength * sneak_hosei * fact << " [Ohm/sq]"<< std::endl;
					}
					checkRN = 0.0;	
					count = 0;
					checkButtonAct = false;
					std::cout << "Current activated probe [left, Right] = [" << A-9 << ", " << B+9 << "]" << std::endl;
					v4 = value * prwidth / prlength * sneak_hosei * fact;
					std::cout << "v4 = " << v4 << " [Mohm/sq]" << std::endl;
					if( v4 < ul && v4 > dl ){
						std::cout << "Good value ------------------" << std::endl;
						vsum += v4;
						vN += 1.0;
					}
				}
				prlength = 5.;
				fact = 1.;
				fixR = vsum / vN;					 
				std::cout << "value = " << fixR/1e+6 << "[Mohm/sq]" << std::endl;
				if( vN > 0 ){
					if( fixR > v_kari && v_kari < 1.0e+7 ) vec_vec[currentRange-scanrange0+(pos[i][0]-1)/9][pos[i][0]-(pos[i][0]-1)/9*9-1] = v_kari/1e+6;	
					else vec_vec[currentRange-scanrange0+(pos[i][0]-1)/9][pos[i][0]-(pos[i][0]-1)/9*9-1] = fixR/1e+6;
					mu = (mu+fixR)/2.;
				}else{
				}
				prlength = 5;
				fact = 1.0;
				loopcount = 0;
				countLimit = 5;
			}
			loopcount++;
			checkButtonAct = false;
			fixR = 0.0;
			tempR = 0.0;
			tempRN = 0.0;
			count = 0;
		}

		for(int i=0; i<check_pos.size(); i++){
			//if( i==0 ) pos.erase( pos.begin() + check_pos.at(i) );
			//else pos.erase( pos.begin() + check_pos.at(i)-i );
		}

		//tr->Fill();
		prlength = 5;
		fact = 1.0;
		v_kari = 0.0;
		
		// Only to get mean and rms for hoteling method .....		
		delete h1;
		h1 = new TH1D("Ohmdist","Ohmdist",60, 0.0, 6.0);
		for(int i=0; i<currentRange+probeRange; i++){//i=0; i<10+currentRange; i++
			for(int j=1; j<10; j++){//j=0; j<10; j++
				if( vec_vec[i][j-1] > 0.01 ) h1->Fill(vec_vec[i][j-1]);	
			}
		}
		double v_mean = h1->GetMean();
		double v_rms = h1->GetRMS();

		delete h1;
		delete h2;
		int N_samp = 0;
		int N_acce = 0;
		double v_max = 0;
		double v_min = 100;
		h1 = new TH1D("Ohmdist","Ohmdist",60, 0.0, 6.0);
		h2 = new TH2D("OhmMap","OhmMap",currentRange+probeRange, 0, (currentRange+probeRange)*5, 9, 0, 9*5);
		for(int i=0; i<currentRange+probeRange; i++){//i=0; i<10+currentRange; i++
      			// 0 line checking ....
			for(int s=1; s<10; s++){
				if( vec_vec[i][s-1] == 0.0 ) lineCheck++;
			}
			for(int j=1; j<10; j++){//j=0; j<10; j++
				if( vec_vec[i][j-1] > 0.01 ) N_samp++;
				if( vec_vec[i][j-1] > target_R/criteria_SF_l && vec_vec[i][j-1] < target_R*criteria_SF_u ) N_acce++;
				if( vec_vec[i][j-1] > v_max ) v_max = vec_vec[i][j-1];
				if( vec_vec[i][j-1] < v_min && vec_vec[i][j-1] > 0.01) v_min = vec_vec[i][j-1];
				if( vec_vec[i][j-1] > 0.01 ) h1->Fill(vec_vec[i][j-1]);
				double v_est=0.0;
				double count_est=0.0;
				double ax = pow((vec_vec[i][j-1]-v_mean)/v_rms, 2);
				double avalue = TMath::ChisquareQuantile(0.95, 1);
				if( ax > avalue && lineCheck < 5 && !lockR && count_Arbi>2){
				//if( vec_vec[i][j-1] > 0.01 && vec_vec[i][j-1] > ul*0.8 ){
					count_Arbi = 0;
					if( i > 0 ){ 
						if( vec_vec[i-1][j-1] > 0.01 && pow((vec_vec[i-1][j-1]-v_mean)/v_rms, 2) < avalue) v_est += vec_vec[i-1][j-1];
						if( vec_vec[i-1][j-1] > 0.01 && pow((vec_vec[i-1][j-1]-v_mean)/v_rms, 2) < avalue) count_est += 1.0;
					}
					if( i < currentRange+probeRange-1 ){ 
						if( vec_vec[i+1][j-1] > 0.01 && pow((vec_vec[i+1][j-1]-v_mean)/v_rms, 2) < avalue ) v_est += vec_vec[i+1][j-1];
						if( vec_vec[i+1][j-1] > 0.01 && pow((vec_vec[i+1][j-1]-v_mean)/v_rms, 2) < avalue ) count_est += 1.0;
					}
					if( j > 1 ){ 
						if( vec_vec[i][j-2] > 0.01 && pow((vec_vec[i][j-2]-v_mean)/v_rms, 2) < avalue ) v_est += vec_vec[i][j-2];
						if( vec_vec[i][j-2] > 0.01 && pow((vec_vec[i][j-2]-v_mean)/v_rms, 2) < avalue ) count_est += 1.0;
					}
					if( j < 9 ){ 
						if( vec_vec[i][j] > 0.01 && pow((vec_vec[i][j]-v_mean)/v_rms, 2) < avalue ) v_est += vec_vec[i][j];
						if( vec_vec[i][j] > 0.01 && pow((vec_vec[i][j]-v_mean)/v_rms, 2) < avalue ) count_est += 1.0;
					}
      					h2->SetBinContent(i+1, 10-j, (v_est+vec_vec[i][j-1])/(count_est+1));// /1000 000
					vec_vec[i][j-1] = (v_est+vec_vec[i][j-1])/(count_est+1);
      					//h2->SetBinContent(i+1, 10-j, vec_vec[i][j-1]);// /1000 000
				}else{
      					h2->SetBinContent(i+1, 10-j, vec_vec[i][j-1]);// /1000 000
				}	
      				//h2->SetBinContent(i+1, 10-j, vec_vec[i][j-1]);// /1000 000
				
			}
			lineCheck = 0;
		}
    double acce_ratio = N_acce*1.0/N_samp*100;
		std::vector<int> check_ohm;
    	  	int max = 0;
      		TString grade_ohm;
      		if( v_mean > target_R/criteria_SF_l && v_mean < target_R*criteria_SF_u){
      			check_ohm.push_back(0);
      		}else{
      			check_ohm.push_back(1);
      			max = 2;
      		}
      		if( acce_ratio > 99.0 ){
      			check_ohm.push_back(0);
      		}else if( acce_ratio > 95.0 ){
      			check_ohm.push_back(1);
      			max = 1;
      		}else{
      			check_ohm.push_back(2);
      			max = 2;
      		}
      
		if( max == 0 ){					
      			grade_ohm = "Resistivity_Grade   : A ";
      		}else if( max == 1 ){
      			grade_ohm = "Resistivity_Grade   : B ";
      		}else if( max == 2 ){
      			grade_ohm = "Resistivity_Grade   : C ";
      		}
      
		std::cout << "Mean :" << v_mean << "[Mohm/sq]" << std::endl;
    	  	std::cout << "RMS :" << v_rms << "[Mohm/sq]" << std::endl;
      		std::cout << "MAX/MIN :" << v_max << "/" << v_min << "[Mohm/sq]" << std::endl;
      		std::cout << "Acceptable :" << acce_ratio << "[%]" << std::endl;
      		std::cout << grade_ohm << std::endl;
      		if( QC_loc == "CERN" ){
      			r_compare compare(comparisonfile);
      			compare.map_ratio();
      			std::cout << "Mean_Ratio[CERN/JP] : " << compare.mean_ratio(v_mean) << std::endl;
      			compare.sheetUpdate();
      			std::cout << "RMS_Ratio[CERN/JP] : " << compare.rms_ratio(v_rms) << std::endl;
      			compare.sheetUpdate();
      			if( preh1 ){
				canv_preDist->cd(1);
      				preh1->Draw();
				canv_preDist->cd(2);
      				preh2->SetStats(0);
      				preh2->GetZaxis()->SetRangeUser(0,3.0);
				gStyle->SetPaintTextFormat("3.2f");
      				preh2->Draw("colz text45");
      				preh2->SetMarkerSize(2);
      			}else{
      				preh2 = new TH2D("preOhmMap","preOhmMap",3+currentRange,0,3+currentRange,10,0,10);
      				preh1 = new TH1D("preOhmdist","preOhmdist",30, 0.0, 3.0);
			}
	
		}
		
		v_mean = h1->GetMean();
		v_rms = h1->GetRMS();
		pos.erase( pos.begin(), pos.end() );
		for(int i=currentRange; i<probeRange+currentRange; i++){//i=0; i<10+currentRange; i++
			for(int s=1; s<10; s++){
				if( vec_vec[i][s-1] == 0.0 ) lineCheck++;
			}
			for(int j=1; j<10; j++){//j=0; j<10; j++
				double ax = (pow((vec_vec[i][j-1]-v_mean)/v_rms, 2));
				double avalue = TMath::ChisquareQuantile(0.95, 1);
      				//if( vec_vec[i][j-1] < v_mean-5*v_rms || vec_vec[i][j-1] > v_mean+3*v_rms || vec_vec[i][j-1]==0.0 ){
      				if( (ax > avalue && lineCheck < 8) || vec_vec[i][j-1] < 0.0){
					int A = (i-currentRange+scanrange0)*9 + j;
					int B = A + 9 ;
					bool overlap = false;
					for(int l=0; l<pos.size(); l++){
						if( A == pos[l][0] ) overlap = true;
					}
					if( !overlap ){
						pos_v.push_back(A);
						pos_v.push_back(B);
						pos.push_back(pos_v);
						pos_v.erase( pos_v.begin(), pos_v.end() );
					}
				}
			}
			lineCheck = 0.0;
		}
		currentRange += probeRange;//+10
		std::cout << "[ Strange point ]" << std::endl;	
		for(int i=0; i<pos.size(); i++){
			double er_value = vec_vec[currentRange-probeRange-scanrange0+(pos[i][0]-1)/9][pos[i][0]-(pos[i][0]-1)/9*9-1];
			std::cout << "[" << pos[i][0] << ", " << pos[i][1] << "] : " << er_value << "[Mohm/sq]" << std::endl;	
		}
		if( pos.size() == 0 ) lockR = true;


		canv_Dist->cd(1);
		h1->Draw();
		canv_Dist->cd(2);
		h2->SetStats(0);
		gStyle->SetPaintTextFormat("3.2f");
		h2->GetZaxis()->SetRangeUser(0,3.0);
		h2->Draw("colz text45");
		h2->SetMarkerSize(2);
		canv->cd();
	}	

	return;
}



////////////////////////////
////////////////////////////
void Rtool::getOhmMap(){
      
	pos.erase( pos.begin(), pos.end() );
      
	relay->change(1,10);
      	prlength = 5;
       	int scanrange = tg_gOMrange->GetNumber()-1; 
       	scanrange0 = tg_gOMrange0->GetNumber()-1;	
	if( scanrange > 10 || scanrange < 0){
		std::cout << "No valid scan range" << std::endl;
		return;
      	}else if( scanrange <= 10 && scanrange >= 0 && !(currentRange == 0) && isRemeasure){
      		for(int s=0; s<probeRange; s++){
			vec_vec.pop_back();
      		}
		currentRange -= probeRange;
      	}else if( scanrange <= 10 && scanrange >= 0 && currentRange == 0 && isRemeasure){
		return;
	}
	if( scanrange - scanrange0 < 0 ) {
		std::cout << "No valid range selected !!" << std::endl;
		return;
	}

	if( isRemeasure ){
		tg_remeasure->SetOn(false);
		SetRemeasure();
	}
	
	probeRange = scanrange - scanrange0 + 1;

	count_Arbi = 0;
	lockR = false;
      
	//for Auto Range fix && getOhmMap--------------------------------
	int count = 0;
      	int loopcount = 0;
      	double v_kari = 0.0;
      	for(int i=0; i<probeRange; i++){//i=0;i<3;i++
		for(int j=1; j<10; j++){//j=0; j<10; j++
			int A = (i+scanrange0)*9 + j;
			int B = A + 9 ;
			for(int s=0; s<5; s++){
				R[s] = 0.0;
				R2[s] = 0.0;
			}
			R[4] = 1.0e+10;
			relay->change(A, B);//(Left, Right)
			checkButtonAct = true;
			while(checkRN < 1 && count < countLimit){
				usleep(100*1000);
				checkRN = tempRN;
				count++;
				std::cout << "[" << A << ", " << B << "] : " << count << "/" << countLimit << std::endl;
				std::cout << value * prwidth / prlength * sneak_hosei * fact << " [Ohm/sq]"<< std::endl;
			}
			fixR = value * prwidth / prlength * sneak_hosei * fact;
			double ul = mu+3*sd;
			double dl = mu-3*sd;
			if( dl < 1.0 ) dl = 1.0;
			if( fixR > dl && fixR < ul ){//fixR < 5.0e+6
	    			vec.push_back(fixR/1e+6);
				mu = (mu+fixR)/2.;
				prlength = 5;
				fact = 1.0;
				loopcount = 0;
				countLimit = 5;
				hs->Fill(fixR);// for getting current mean and std ...
			}else if( loopcount < 2 && j > 1 ){
				j--;
				v_kari = fixR;
			}else if( loopcount < 2 && j == 1 && i > 0){
				i--;
				j = 9;
				v_kari = fixR;
			}else if( loopcount < 2 && j == 1 && i == 0){
				j--;
				v_kari = fixR;
			}else if( loopcount >= 2 && fixR > 1.0e+7){ 
				if( fixR > v_kari && v_kari < 1.0e+7 ) vec.push_back(v_kari/1e+6);
				else vec.push_back(0.0);
				pos_v.push_back(A);
				pos_v.push_back(B);
				pos.push_back(pos_v);
				pos_v.erase( pos_v.begin(), pos_v.end() );
				prlength = 5;
				fact = 1.0;
				loopcount = 0;
				countLimit = 5;
				lineCheck += 1;
				if( fixR > v_kari && v_kari < 1.0e+7 ) hs->Fill(fixR);// for getting current mean and std ...
			}else if( loopcount >= 2 && fixR < 1.0e+7){
				if( fixR > v_kari && v_kari < 1.0e+7 ) vec.push_back(v_kari/1e+6);
				else vec.push_back(fixR/1e+6);
				//vec.push_back(fixR/1e+6);
				pos_v.push_back(A);
				pos_v.push_back(B);
				pos.push_back(pos_v);
				pos_v.erase( pos_v.begin(), pos_v.end() );
				//h1->Fill(fixR/1e+6);
				mu = (mu+fixR)/2.;
				prlength = 5;
				fact = 1.0;
				loopcount = 0;
				countLimit = 5;
				if( fixR > v_kari && v_kari < 1.0e+7 ) hs->Fill(fixR);// for getting current mean and std ...
			}
			loopcount++;
			checkButtonAct = 0.0;
			fixR = 0.0;
			tempR = 0.0;
			tempRN = 0.0;
			count = 0;
			checkRN = 0.0;
			if( hs->Integral() > 2 ){
				mu = hs->GetMean();
				sd = hs->GetRMS();	
			}
		}
		vec_vec.push_back(vec);
		if( vec_vec.size() != i+currentRange+1 && vec_vec.size() > 0) vec_vec.pop_back();/////okyustoti
		//tr->Fill();
		vec.erase( vec.begin(), vec.end() );
	
		// Remove 0 line 
		if( lineCheck >= 8 ){
			for(int lc=0; lc<9; lc++){
				pos.pop_back();
			}
		}
		lineCheck = 0;
	}
    
/*	
	// Re-measure points which returned Strange value
	std::vector<int> check_pos;
      	for(int i=0; i<pos.size(); i++){
      		int A = pos[i][0];
      		int B = pos[i][1];
      		//double er_value = vec_vec[currentRange-probeRange+(pos[i][0]-1)/9][pos[i][0]-(pos[i][0]-1)/9*9-1];
		for(int s=0; s<5; s++){
      			R[s] = 0.0;
      			R2[s] = 0.0;
      		}
      		R[4] = 1.0e+10;
      		relay->change(A, B);//(Left, Right)
      		checkButtonAct = true;
		while(checkRN < 1 && count < countLimit){
		      	usleep(100*1000);
      			checkRN = tempRN;
      			count++;
      			std::cout << "[" << A << ", " << B << "] : " << count << "/" << countLimit << std::endl;
      			std::cout << value * prwidth / prlength * sneak_hosei * fact << " [Ohm/sq]"<< std::endl;
      		}
      		fixR = value * prwidth / prlength * sneak_hosei * fact;
      		double ul = mu+3*sd;
      		double dl = mu-3*sd;
      		if( dl < 1.0 ) dl = 1.0;
      		if( fixR > dl && fixR < ul ){//fixR < 5.0e+6
      			vec_vec[currentRange-scanrange0+(pos[i][0]-1)/9][pos[i][0]-(pos[i][0]-1)/9*9-1] = fixR/1e+6;
      			mu = (mu+fixR)/2.;
      			prlength = 5;
      			fact = 1.0;
      			loopcount = 0;
      			countLimit = 5;
      			check_pos.push_back(i);
			hs->Fill(fixR);// for getting current mean and std ...
      		}else if( loopcount < 2){
      			v_kari = fixR;
      			i--;
      		}else if( loopcount >= 2 ){
      			double v1,v2,v3,v4;
      			double vN = 0.0;
      			double vsum = 0.0;      
			//v1
			if( A > 0 && B <= (probeRange+1)*9 ){ 
      				fact = 1.0;
      				relay->change(A, B);	
      				usleep(1000*1000);
      				std::cout << "Current activated probe [left, Right] = [" << A << ", " << B << "]" << std::endl;
      				v1 = value * prwidth / prlength * sneak_hosei * fact;
      				std::cout << "v1 = " << v1 << " [Mohm/sq]" << std::endl;
      				if( v1 < ul && v1 > dl ){
      					std::cout << "Good value ------------------" << std::endl;
      					vsum += v1;
      					vN += 1.0;		
      				}
      			}				
			//v2
			if( A > 0 && B <= probeRange*9 ){ 
			      	prlength = 10;
      				fact = sneak_hosei10;
      				relay->change(A, B+9);	
      				usleep(1000*1000);
      				std::cout << "Current activated probe [left, Right] = [" << A << ", " << B+9 << "]" << std::endl;
      				v2 = value * prwidth / prlength * sneak_hosei * fact;
      				std::cout << "v2 = " << v2 << " [Mohm/sq]" << std::endl;
      				if( v2 < ul && v2 > dl ){
      					std::cout << "Good value ------------------" << std::endl;
      					vsum += v2;
      					vN += 1.0;
      				}
      			}
			//v3
			if( A > 9 && B <= (probeRange+1)*9 ){ 
			      	prlength = 10;
      				fact = sneak_hosei10;
      				relay->change(A-9, B);	
      				usleep(1000*1000);
      				std::cout << "Current activated probe [left, Right] = [" << A-9 << ", " << B << "]" << std::endl;
      				v3 = value * prwidth / prlength * sneak_hosei * fact;
      				std::cout << "v3 = " << v3 << " [Mohm/sq]" << std::endl;
      				if( v3 < ul && v3 > dl ){
      					std::cout << "Good value ------------------" << std::endl;
      					vsum += v3;
      					vN += 1.0;
      				}
      			} 
			//v4
		        if( A > 9 && B <= probeRange*9 ){
			      	prlength = 15;
      				fact = sneak_hosei15;
      				relay->change(A-9, B+9);	
      				usleep(1000*1000);
      				std::cout << "Current activated probe [left, Right] = [" << A-9 << ", " << B+9 << "]" << std::endl;
      				v4 = value * prwidth / prlength * sneak_hosei * fact;
      				std::cout << "v4 = " << v4 << " [Mohm/sq]" << std::endl;
      				if( v4 < ul && v4 > dl ){
      					std::cout << "Good value ------------------" << std::endl;
      					vsum += v4;
      					vN += 1.0;
      				}
      			}
     
			prlength = 5.;
      			fact = 1.;
      
			fixR = vsum / vN;
      			std::cout << "value = " << fixR/1e+6 << "[Mohm/sq]" << std::endl;
      
			if( vN > 0 ){
      				if( fixR > v_kari && v_kari < 1.0e+7 ) vec_vec[currentRange-scanrange0+(pos[i][0]-1)/9][pos[i][0]-(pos[i][0]-1)/9*9-1] = v_kari/1e+6;
      				else vec_vec[currentRange-scanrange0+(pos[i][0]-1)/9][pos[i][0]-(pos[i][0]-1)/9*9-1] = fixR/1e+6;
				if( fixR > v_kari && v_kari < 1.0e+7 ) hs->Fill(fixR);// for getting current mean and std ...
      				mu = (mu+fixR)/2.;
      				check_pos.push_back(i);
      			}else{
      			}
      			prlength = 5;
      			fact = 1.0;
      			loopcount = 0;
      			countLimit = 5;
			if( hs->Integral() > 2 ){
				mu = hs->GetMean();
				sd = hs->GetRMS();	
			}
      		}
      
		loopcount++;
      		checkButtonAct = false;
      		fixR = 0.0;
      		tempR = 0.0;
      		tempRN = 0.0;
      		count = 0;
      		checkRN = 0.0;	
      	}
      
	for(int i=0; i<check_pos.size(); i++){
      		pos.erase( pos.begin() + check_pos[i] );
	}
*/

      	//tr->Fill();
	prlength = 5;
      	fact = 1.0;
      	v_kari = 0.0;
      
	delete h1;
      	delete h2;
      	delete hs;
      	int N_acce = 0;
      	double v_max = 0;
      	double v_min = 100;
      	h1 = new TH1D("Ohmdist","Ohmdist",60, 0.0, 6.0);
      	h2 = new TH2D("OhmMap","OhmMap",probeRange+currentRange, 0, (probeRange+currentRange)*5, 9, 0, 9*5);
      	hs = new TH1D("","",60, 0.0, 6e+6);//bin 100
      	int N_samp = 0;
      	for(int i=0; i<probeRange+currentRange; i++){//i=0; i<10+currentRange; i++
      		for(int j=1; j<10; j++){//j=0; j<10; j++
      			if( vec_vec[i][j-1] > 0.01 ) N_samp++;
      			if( vec_vec[i][j-1] > target_R/criteria_SF_l && vec_vec[i][j-1] < target_R*criteria_SF_u ) N_acce++;
      			if( vec_vec[i][j-1] > v_max ) v_max = vec_vec[i][j-1];
      			if( vec_vec[i][j-1] < v_min && vec_vec[i][j-1] > 0.01) v_min = vec_vec[i][j-1];
      			h2->SetBinContent(i+1, 10-j, vec_vec[i][j-1]);// /1000 000
      			if( vec_vec[i][j-1] > 0.01 ) h1->Fill(vec_vec[i][j-1]);
      			if( vec_vec[i][j-1] > 0.01 ) hs->Fill(vec_vec[i][j-1]*1e+6);
      		}
      	}
      	double v_mean = h1->GetMean();
      	double v_rms = h1->GetRMS();
      	double acce_ratio = N_acce*1.0/N_samp*100;
      	for(int i=currentRange; i<probeRange+currentRange; i++){//i=0; i<10+currentRange; i++
      		// 0 line checking ....
		for(int s=1; s<10; s++){
			if( vec_vec[i][s-1] == 0.0 ) lineCheck++;
		}
      		for(int j=1; j<10; j++){//j=0; j<10; j++
			double ax = (pow((vec_vec[i][j-1]-v_mean)/v_rms, 2));
			double avalue = TMath::ChisquareQuantile(0.95, 1);
      			//if( vec_vec[i][j-1] < v_mean-5*v_rms || vec_vec[i][j-1] > v_mean+3*v_rms || vec_vec[i][j-1]==0.0 ){
      			if( ax > avalue && lineCheck < 8 ){
				int A = (i-currentRange+scanrange0)*9 + j;
				int B = A + 9 ;
				bool overlap = false;
				for(int l=0; l<pos.size(); l++){
					if( A == pos[l][0] ) overlap = true;
				}
				if( !overlap ){
					pos_v.push_back(A);
					pos_v.push_back(B);
					pos.push_back(pos_v);
					pos_v.erase( pos_v.begin(), pos_v.end() );
				}
      			}
      		}
		lineCheck = 0;
	}
      
	std::vector<int> check_ohm;
      	int max = 0;
      	TString grade_ohm;
      	if( v_mean > target_R/criteria_SF_l && v_mean < target_R*criteria_SF_u){
      		check_ohm.push_back(0);
      	}else{
      		check_ohm.push_back(1);
      		max = 2;
      	}
      	if( acce_ratio > 99.0 ){
      		check_ohm.push_back(0);
      	}else if( acce_ratio > 95.0 ){
      		check_ohm.push_back(1);
      		max = 1;
      	}else{
      		check_ohm.push_back(2);
      		max = 2;
      	}
      
	if( max == 0 ){					
      		grade_ohm = "Resistivity_Grade   : A ";
      	}else if( max == 1 ){
      		grade_ohm = "Resistivity_Grade   : B ";
      	}else if( max == 2 ){
      		grade_ohm = "Resistivity_Grade   : C ";
      	}
      
	std::cout << "Mean :" << v_mean << "[Mohm/sq]" << std::endl;
      	std::cout << "RMS :" << v_rms << "[Mohm/sq]" << std::endl;
      	std::cout << "MAX/MIN :" << v_max << "/" << v_min << "[Mohm/sq]" << std::endl;
      	std::cout << "Acceptable :" << acce_ratio << "[%]" << std::endl;
      	std::cout << grade_ohm << std::endl;
      	if( QC_loc == "CERN" ){
      		r_compare compare(comparisonfile);
      		compare.map_ratio();
      		std::cout << "Mean_Ratio[CERN/JP] : " << compare.mean_ratio(v_mean) << std::endl;
      		compare.sheetUpdate();
      		std::cout << "RMS_Ratio[CERN/JP] : " << compare.rms_ratio(v_rms) << std::endl;
      		compare.sheetUpdate();
      		if( preh1 ){
			canv_preDist->cd(1);
      			preh1->Draw();
			canv_preDist->cd(2);
      			preh2->SetStats(0);
      			preh2->GetZaxis()->SetRangeUser(0,3.0);
			gStyle->SetPaintTextFormat("3.2f");
      			preh2->Draw("colz text45");
      			preh2->SetMarkerSize(2);
      		}else{
      			preh2 = new TH2D("preOhmMap","preOhmMap",3+currentRange,0,3+currentRange,10,0,10);
      			preh1 = new TH1D("preOhmdist","preOhmdist",30, 0.0, 3.0);
		}

	}
      
	currentRange += probeRange;//+10
      	canv_Dist->cd(1);
       	h1->Draw();
      	canv_Dist->cd(2);
      	h2->SetStats(0);
      	gStyle->SetPaintTextFormat("3.2f");
      	h2->GetZaxis()->SetRangeUser(0,3.0);
      	h2->Draw("colz text45");
      	//h2->Draw("colz");
	h2->SetMarkerSize(2);
	canv->cd();
      
	std::cout << "[ Strange point ]" << std::endl;	
      	for(int i=0; i<pos.size(); i++){
      		//std::cout << "[" << pos[i][0] << ", " << pos[i][1] << "] : " << std::endl;
      		//std::cout << " [i][j]       [" << currentRange-probeRange-scanrange0+(pos[i][0]-1)/9 << ", " << pos[i][0]-(pos[i][0]-1)/9*9-1 << "] : " << std::endl;
      		double er_value = vec_vec[currentRange-probeRange-scanrange0+(pos[i][0]-1)/9][pos[i][0]-(pos[i][0]-1)/9*9-1];
      		std::cout << "[" << pos[i][0] << ", " << pos[i][1] << "] : " << er_value << "[Mohm/sq]" << std::endl;
       	}
	
	TString Type =  tg_fileType->GetText();
	std::vector<std::vector<int> > vecScan = positionList[Type];		
	scanCount++;
	if( scanCount >= vecScan.size() ) scanCount = vecScan.size()-1;
       	tg_gOMrange->SetNumber(vecScan[scanCount][1]); 
       	tg_gOMrange0->SetNumber(vecScan[scanCount][0]);	

	if( !auto_range ){
       		tg_gOMrange->SetNumber(10); 
       		tg_gOMrange0->SetNumber(1);	
	}


	return;
}
////////////////////////////////////////////
////////////////////////////////////////////
void Rtool::Savefile(){
  
	if( !save_check ){
		f->cd();
		tr->Fill();
		h1->Write();
		h2->Write();
		hs->Write();
		preh1->Write();
		preh2->Write();
		h2HV->Write();
		tg1->Write();
		tg2->Write();
		tr->Write();
		time_t now = std::time(&now);
		struct tm *pnow = localtime(&now);
		int year  = pnow->tm_year+1900;
		int mon  = pnow->tm_mon+1;
		int day  = pnow->tm_mday;
		daytime = day+mon*100+year*10000;
		version = ver; 
		tr_info->Fill();
		tr_info->Write();
		f->Close();
		std::cout << "File saved-----(Enter next file name)" << std::endl;
		currentRange = 0;
		currentRange_HV = 0;
		vec_vec.erase( vec_vec.begin(), vec_vec.end() );
		vec_vec_HV.erase( vec_vec_HV.begin(), vec_vec_HV.end() );
		save_check = true;  
	//}else{
	//	std::cout << "No measurement done" << std::endl;
	}

	return;
}

////////////////////////////////////////////
////////////////////////////////////////////
void Rtool::SetNewfile(){

	if( !save_check && meas_check){
		f->cd();
		tr->Fill();
      		h1->Write();
      		h2->Write();
      		hs->Write();
      		preh1->Write();
      		preh2->Write();
      		h2HV->Write();
      		tg1->Write();
      		tg2->Write();
      		tr->Write();
      		time_t now = std::time(&now);
      		struct tm *pnow = localtime(&now);
      		int year  = pnow->tm_year+1900;
      		int mon  = pnow->tm_mon+1;
      		int day  = pnow->tm_mday;
      		daytime = day+mon*100+year*10000;
      		version = ver;
      		tr_info->Fill();
      		tr_info->Write();
      		f->Close();
      		std::cout << "file saved-----(Enter next file name)" << std::endl;
      		currentRange = 0;
      		currentRange_HV = 0;
      		vec_vec.erase( vec_vec.begin(), vec_vec.end() );
      		vec_vec_HV.erase( vec_vec_HV.begin(), vec_vec_HV.end() );
	}
	
	// Get foil Type (SE1, SE2, ...)
	TString Type =  tg_fileType->GetText();
	// Get foil ID (1~86)
	std::stringstream ss;
	std::stringstream ss_pcb;
	int IDn = 0;
	int IDn_pcb = 0;
	if( QC_loc == "JAPAN" ){
		IDn = tg_fileID->GetNumber();
	}else{
	      	IDn = tg_fileID->GetNumber();
	      	IDn_pcb = tg_pcbID->GetNumber();
	}

	ss << IDn;
	ss_pcb << IDn_pcb;
	TString ID = TString(ss.str());
	TString ID_pcb = TString(ss_pcb.str());
	if( ss.str().size() == 1 ) ID = initial_ID + "000" + ID;
	else if( ss.str().size() == 2 ) ID = initial_ID + "00" + ID;
	if( ss_pcb.str().size() == 1 ) ID_pcb = initial_ID + "0" + ID_pcb;
	else if( ss_pcb.str().size() == 2 ) ID_pcb = initial_ID + ID_pcb;
	
	TString newfilename;
	if (QC_loc =="JAPAN" ){
		newfilename = Type + "_" + ID;
		comparisonfile = newfilename;
	}else{
		newfilename = Type + "_" + ID_pcb;
		comparisonfile = Type + "_" + ID;
	}
      	
	if( QC_loc == "JAPAN" )  file = "../output/" + newfilename + "_ohm.root";
	else file = "../output/" + newfilename + "_pcb_ohm.root";
	FileStat_t info;
	//char c0[32] = file;
	
	std::cout << "New file name is " << file << std::endl;
	//std::cout << "overwrite mode :" << ifOverWrite << std::endl;
      	
	if( !gSystem->GetPathInfo( file, info ) ){
      		std::cout << "'" << file << "'" << " already exists !" << std::endl; 
      		if( !ifOverWrite ){
      			std::cout << "Creatng new file cancelled (To overwrite, check overwrite box)" << std::endl;
      			save_check = true;
      		}else{
      			std::cout << "Overwrite " << newfilename << std::endl;
      			currentRange = 0;
      			currentRange_HV = 0;
      			vec_vec.erase( vec_vec.begin(), vec_vec.end() );
			vec_vec_HV.erase( vec_vec_HV.begin(), vec_vec_HV.end() );
      			f = new TFile(file, "recreate");
      			tr = new TTree("tree","tree");
      			tr->Branch("vec_vec",&vec_vec);
      			tr->Branch("vec_vec_HV",&vec_vec_HV);
      			h2 = new TH2D("OhmMap","OhmMap",3+currentRange,0,3+currentRange,10,0,10);
      			preh2 = new TH2D("preOhmMap","preOhmMap",3+currentRange,0,3+currentRange,10,0,10);
      			h2->GetZaxis()->SetRangeUser(0, 3.0);
      			h2HV = new TH2D("OhmMapfromHV","OhmMapfromHV",11,0,11,9,0,9);
      			h1 = new TH1D("Ohmdist","Ohmdist",30, 0.0, 3.0);//bin 100
      			hs = new TH1D("","",60, 0.0, 6e+6);//bin 100
      			preh1 = new TH1D("preOhmdist","preOhmdist",30, 0.0, 3.0);
      			h2Ca = new TH2D("calibMap","calibMap",11,0,11,9,0,9);
      			tg1 = new TGraphErrors();
      			tg2 = new TGraphErrors();
      			tr_info = new TTree("tree_info","tree_info");
      			tr_info->Branch("daytime",&daytime, "daytime/D");
      			tr_info->Branch("version",&version, "version/D");
      			std::cout << "'" << newfilename << "'" << " has been made----------" << std::endl;
      			save_check = false;
			std::vector<std::vector<int> > vec = positionList[Type];	
       			tg_gOMrange->SetNumber(vec[0][1]); 
       			tg_gOMrange0->SetNumber(vec[0][0]);	
			if( !auto_range ){
		       		tg_gOMrange->SetNumber(10); 
       				tg_gOMrange0->SetNumber(1);	
			}
			scanCount = 0;
			if( ifOverWrite ){
	   			tg_overwrite->SetOn(false);
				SetOverWrite();
			}

		}
	}else{
      		currentRange = 0;
      		currentRange_HV = 0;
      		vec_vec.erase( vec_vec.begin(), vec_vec.end() );
		vec_vec_HV.erase( vec_vec_HV.begin(), vec_vec_HV.end() );
      		f = new TFile(file, "recreate");
      		tr = new TTree("tree","tree");
      		tr_info = new TTree("tree_info","tree_info");
      		tr->Branch("vec_vec",&vec_vec);
 		tr->Branch("vec_vec_HV",&vec_vec_HV);
      		h2 = new TH2D("OhmMap","OhmMap",3+currentRange,0,3+currentRange,10,0,10);
      		preh2 = new TH2D("preOhmMap","preOhmMap",3+currentRange,0,3+currentRange,10,0,10);
      		h2HV = new TH2D("OhmMapfromHV","OhmMapfromHV",11,0,11,9,0,9);
      		h1 = new TH1D("Ohmdist","Ohmdist",30, 0.0, 3.0);
      		hs = new TH1D("","",60, 0.0, 6.0);//bin 100
      		preh1 = new TH1D("preOhmdist","preOhmdist",30, 0.0, 3.0);
      		h2Ca = new TH2D("calibMap","calibMap",11,0,11,9,0,9);
      		tg1 = new TGraphErrors();
		tg2 = new TGraphErrors();
      		tr_info = new TTree("tree_info","tree_info");
      		tr_info->Branch("daytime",&daytime, "daytime/D");
      		tr_info->Branch("version",&version, "version/D");
      		std::cout << newfilename << " has been made----------" << std::endl;
      		relay->change(1,10);
		save_check = false;
		std::vector<std::vector<int> > vec = positionList[Type];		
       		tg_gOMrange->SetNumber(vec[0][1]); 
       		tg_gOMrange0->SetNumber(vec[0][0]);	
		if( !auto_range ){
	       		tg_gOMrange->SetNumber(10); 
       			tg_gOMrange0->SetNumber(1);	
		}
		scanCount = 0;
		if( ifOverWrite ){
   			tg_overwrite->SetOn(false);
			SetOverWrite();
		}
	}
	return;
}
