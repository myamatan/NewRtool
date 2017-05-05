// Relay (Arduino)
class Relay {
public:
	Relay( std::string RS232_port_name );
	virtual ~Relay();

	int init();
	void change( unsigned int param_1, unsigned int param_2 );

private:
	std::string _RS232_port_name;

	boost::asio::io_service		_ioService;
	boost::asio::serial_port	_device;
};

typedef boost::shared_ptr<Relay>        RelayPtr;

Relay::Relay( std::string RS232_port_name ) :
		_RS232_port_name( RS232_port_name ),
		_device( _ioService, _RS232_port_name )
{

}

Relay::~Relay(){
	if( _device.is_open() ) _device.close();
}

int Relay::init(){
	if( _device.is_open() ) _device.close();
	try {
		_device.open( _RS232_port_name );
		_device.set_option( boost::asio::serial_port::baud_rate(9600) );
		_device.set_option( boost::asio::serial_port::parity( boost::asio::serial_port::parity::none ) );
		_device.set_option( boost::asio::serial_port::character_size( boost::asio::serial_port::character_size(8) ) );
		_device.set_option( boost::asio::serial_port::stop_bits( boost::asio::serial_port::stop_bits::one ) );
		_device.set_option( boost::asio::serial_port_base::flow_control( boost::asio::serial_port_base::flow_control::none ) );
	}
	catch( std::system_error & error ){
		std::cout 	<< "RS232 port: " 
					<< _RS232_port_name
					<< "initialisation failed:" 
					<< std::endl;
	    std::cout   << error.what()   << std::endl;
		return -1;
	}
	return 0;		
}

void Relay::change( unsigned int param_1, unsigned int param_2 ){
	if( QC_loc == "CERN" ){	
		if(param_1 == 84 || param_2==57 || param_2==81 || param_2==82){//CERN relay version
			int a = param_1;
			int b = param_2;
			param_1 = b;
			param_2 = a;
		}
	}else if( QC_loc == "JAPAN" ){
		if( param_1==84 || param_2==9 || param_2==10 || param_2==11 || param_2==12
				|| param_2==13 || param_2==14 || param_2==15 || param_2==16
				|| param_2==57 || param_2==63 || param_2==74 || param_2==81
				|| param_2==82 || param_2==88 ){
			int a = param_1;
			int b = param_2;
			param_1 = b;
			param_2 = a;
		}
	}
	// convert and format command	
	char buf1[12];
	char buf2[12];
	sprintf( buf1, "%u", param_1 );
	sprintf( buf2, "%u", param_2 );
			       
	std::string toSend = std::string( buf1 ) + "." + std::string( buf2 ) + ".";
	//std::string toSend = std::string( buf1 ) + "." + std::string( buf1 ) + "."
	//			+ std::string( buf2 ) + "." + std::string( buf2 ) + "."; // Why???????????
	//std::cout << toSend << std::endl;
	// send command
	_device.write_some( boost::asio::buffer( toSend ) );
}
