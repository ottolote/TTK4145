#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

class handler {
	public:
		void schmandler() {
			std::cout << "schmandler schschmandler\n";
		}

		void threadfunc(boost::asio::io_service *io) {
			// doStuff();
			boost::asio::deadline_timer t(*io, boost::posix_time::seconds(1));

			io->post( boost::bind(&handler::schmandler,this));
		}

};

int main() {
	boost::asio::io_service io;

	std::cout << "Before io.run()\n";

	boost::thread t(boost::bind(&handler::threadfunc, &io));
	io.run();

	std::cout << "io.run() done\n";

	return 0;
}

