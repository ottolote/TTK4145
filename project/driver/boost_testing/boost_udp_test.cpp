#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#define LISTENPORT 31337

using boost::asio::ip::udp

int main(){
	try{
		boost::asio::io_service io;
		udp::resolver resolver(io);
		udp::resolver::query query(udp::v4(), '0', LISTENPORT);
		udp::endpoint receiver_endpoint = *resolver.resolve(query);

		udp::socket socket(io);
		socket.open(udp::v4());

		boost::array<char, 1> send_buf = {{ 0 }};
		socket.send_to(boost::buffer(send_buf), reciever_endpoint);

		boost::array<char, 128> recv_buf;
		udp::endpoint sender_endpoint;
		size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
		std::cout.write(recv_buf.data(), len);
	}

	catch (std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	return 0;
}