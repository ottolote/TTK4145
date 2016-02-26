#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
//#include <pthread.h>

using boost::asio::ip::udp;

int main(){
	try{
		char address[] = "localhost";
		char service[] = "20003";


		boost::asio::io_service io;
		udp::resolver resolver(io);
		udp::resolver::query query(udp::v4(), address, service);
		udp::endpoint receiver_endpoint = *resolver.resolve(query);

		udp::socket socket(io);
		socket.open(udp::v4());

		boost::array<char, 1> send_buf = {{ 0 }};
		socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

		boost::array<char, 128> recv_buf;
		udp::endpoint sender_endpoint;
		std::cout << "over\n";
		size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
		std::cout << "under\n";
		std::cout.write(recv_buf.data(), len);
	}

	catch (std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
