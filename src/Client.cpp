//#include "../include/SocketHandler.hpp"
#include <iostream>
#include <utility>
#include <boost/bind.hpp>
#include "../include/Client.hpp"
#include <boost/thread.hpp>

//namespace aliasing to enhance readability
////Note: must be sure that namespaces do not clash 
using namespace boost;
using namespace boost::asio;
//constants in anon namespace good for encapsulation
namespace {
	const std::string IP = "127.0.0.1";
	const std::string PORT = "3000";
	const size_t BUFFER_SIZE = 64;
}


Client::Client(boost::asio::io_service& io_service)
	: socket_{io_service}, strand_read{io_service}, strand_write{io_service},
		incomingMessages{new std::queue<std::string>{}}, outgoingMessages{new std::queue<std::string>{}}
		{}

void Client::start(tcp::resolver::iterator endpoint_iter) {
    // Start the connect actor.
         start_connect(endpoint_iter);
}

void Client::stop() {
    socket_.close();
}

void Client::start_connect(tcp::resolver::iterator endpoint_iter) {
	if(endpoint_iter != tcp::resolver::iterator()) {
		std::cout << "Trying " << endpoint_iter->endpoint() << "..\n";
		// Start the asynchronous connect operation.
		 socket_.async_connect(endpoint_iter->endpoint(),
		 boost::bind(&Client::handle_connect, this, _1, endpoint_iter));	}
	else {
		//No more enpoints
		stop();
	}
}

void Client::handle_connect(const boost::system::error_code& ec,
	tcp::resolver::iterator endpoint_iter) {
	if (ec) {
      		std::cout << "Connect error: " << ec.message() << "\n";

      // We need to close the socket used in the previous connection attempt
      //       // before starting a new one.
       		socket_.close();
      // Try the next available endpoint.
       		start_connect(++endpoint_iter);
        } else {
		std::cout << "Connected to " << endpoint_iter->endpoint() << "\n";
      	        boost::asio::ip::tcp::no_delay option(true);
		socket_.set_option(option);
	//read and write operations in separate strands to run concurrently 
	//use strands not threads so no need to lock the socket 
               	strand_read.post(strand_read.wrap(boost::bind(&Client::start_read, this)));
	 	strand_write.post(strand_write.wrap(boost::bind(&Client::start_write, this)));
 	}
}

void Client::start_read() {
    //         // Start an asynchronous operation to read a newline-delimited message.
                boost::asio::async_read_until(socket_, input_buffer_, '\n',
	                boost::bind(&Client::handle_read, this, _1));
}

void Client::handle_read(const boost::system::error_code& ec) {
	if (!ec) {
      // Extract the newline-delimited message from the buffer.
         	std::string line;
                std::istream is(&input_buffer_);
                std::getline(is, line);
	//put into incoming message queue
		incomingMessages->push(line);
      // Empty messages are heartbeats and so ignored.
     	        if (!line.empty())   {
                    	 std::cout << "Received: " << line << "\n"; 
                }
                start_read();
        }  else {
         	 std::cout << "Error on receive: " << ec.message() << "\n";
      		 stop();
        }        
}

void Client::start_write() {
     //Start an asynchronous operation to send the users input after pressing ENTER.
        	std::string line; 
		std::getline(std::cin, line);
		outgoingMessages->push(line);
		line += "\n";
           boost::asio::async_write(socket_, boost::asio::buffer(line, BUFFER_SIZE),
           boost::bind(&Client::handle_write, this, _1));
}

void Client::handle_write(const boost::system::error_code& ec) {
	if(!ec) {
		start_write();
	} else {
		std::cout << ec.message() << "\n";
		stop();
	}
}


int main() {
	try {
    		boost::asio::io_service io_service;
    		tcp::resolver r(io_service);
    		Client c(io_service);

    		c.start(r.resolve(tcp::resolver::query(IP, PORT)));
		
		boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
    		io_service.run();
		t.join();
  	} catch (std::exception& e) {
    		std::cerr << "Exception: " << e.what() << "\n";
  	}
  	return 0;
}

