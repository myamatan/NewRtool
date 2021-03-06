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
	{"SE5", {{2,10},{1,9}} },
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

void Rtool_position(){
	
	TString file = "SE6";
	std::vector<std::vector<int> > vec = positionList[file];

	std::cout << "Board type :" << file << std::endl;
	std::cout << "Number of positioning :" << vec.size() << std::endl;
	for(int i=0; i<vec.size(); i++){
		std::cout << "from " << vec[i][0] << " to " << vec[i][1] << std::endl;
	}
}
