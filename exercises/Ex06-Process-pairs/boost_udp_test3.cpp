#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::udp;


std::string make_daytime_string() {
	return "test\n";
}


class udp_server {
	public: 
		udp_server(boost::asio::io_service& io)
			: socket_(io, udp::endpoint(udp::v4(), 20003))
		{
			start_receive();
		}
	private:
		void start_receive() {
			socket_.async_receive_from(
				boost::asio::buffer(recv_buffer_), remote_endpoint_,
				boost::bind(&udp_server::handle_receive, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}

		void handle_receive(const boost::system::error_code& error,
			std::size_t) //bytes transferred
		{
			if(!error || error == boost::asio::error::message_size) {
				boost::shared_ptr<std::string> message(
					new std::string(make_daytime_string()));
				socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
					boost::bind(&udp_server::handle_send, this, message,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));

				start_receive();
			}
		}
			

		void handle_send(boost::shared_ptr<std::string>, //message
			const boost::system::error_code&, //error
			std::size_t) // bytes transferred
		{
		}

		udp::socket socket_;
		udp::endpoint remote_endpoint_;
		boost::array<char, 1> recv_buffer_;
	};

		




int main() {
	try{
		boost::asio::io_service io;
		udp_server server(io);
		io.run();
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
