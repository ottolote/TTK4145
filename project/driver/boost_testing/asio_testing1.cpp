#include <iostream>
#include <boost/asio.hpp>

int main(){
	boost::asio::io_service io;
	
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));

	t.wait();

	std::cout << "Timer expired\n";

	return 0;
}

