#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>
using namespace boost::property_tree;

ptree ini_reader;

void read_config(ptree pt){
	
	// Device fluke
	if (boost::optional<std::string> str = pt.get_optional<std::string>("Config.device_fluke")) {
		std::cout << "device_fluke : " << str.get() << std::endl;
		device_P = str.get();
	}
	else {
		std::cout << "device_fluke is nothing" << std::endl;
	}
	// Device Relay (Arduino Uno)
	if (boost::optional<std::string> str = pt.get_optional<std::string>("Config.device_relay")) {
		std::cout << "device_relay : " << str.get() << std::endl;
		device_relay_P = str.get();
	}
	else {
		std::cout << "device_relay is nothing" << std::endl;
	}
	// QC location (CERN or JAPAN)
	if (boost::optional<std::string> str = pt.get_optional<std::string>("Config.QC_location")) {
		std::cout << "QC_location : " << str.get() << std::endl;
		QC_loc = str.get();
	}
	else {
		std::cout << "QC_location is nothing" << std::endl;
	}
	
	// Target resistivity 
	if (boost::optional<double> value = pt.get_optional<double>("Config.target_resistivity")) {
		std::cout << "Target resistivity : " << value.get() << std::endl;
		target_R = value.get();
	}
	else {
		std::cout << "Target resistivity is nothing" << std::endl;
	}
	// Upper limit SF
	if (boost::optional<double> value = pt.get_optional<double>("Config.upper_limit")) {
		std::cout << "Upeer limit : " << value.get() << std::endl;
		criteria_SF_u = value.get();
	}
	else {
		std::cout << "Upper limit is nothing" << std::endl;
	}
	// Lower limit SF
	if (boost::optional<double> value = pt.get_optional<double>("Config.lower_limit")) {
		std::cout << "Lower limit : " << value.get() << std::endl;
		criteria_SF_l = value.get();
	}
	else {
		std::cout << "Lower limit is nothing" << std::endl;
	}
	
	// Auto range
	if (boost::optional<bool> boo = pt.get_optional<bool>("Config.auto_range")) {
		std::cout << "Auto range : " << boo.get() << std::endl;
		auto_range = boo.get();
	}
	else {
		std::cout << "Auto range is nothing" << std::endl;
	}

	// Initial ID (0 for nominal, 1 for reproduction
	if (boost::optional<std::string> str = pt.get_optional<std::string>("Config.initial_id")) {
		std::cout << "Initial_ID : " << str.get() << std::endl;
		initial_ID = str.get();
	}
	else {
		std::cout << "initial_id is nothing" << std::endl;
	}

}

