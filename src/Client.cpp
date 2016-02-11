//#include "../include/SocketHandler.hpp"
#include <iostream>
#include <utility>
#include <boost/bind.hpp>
#include "../include/Client.hpp"

//namespace aliasing to enhance readability
////Note: must be sure that namespaces do not clash 
using namespace boost;
using namespace boost::asio;
//using namespace boost::asio::ip;
//constants in anon namespace good for encapsulation
namespace {
	const std::string IP = "127.0.0.1";
	const std::string PORT = "3000";
}


Client::Client(boost::asio::io_service& io_service): messagesOut{}, messagesIn{}, socket_{io_service} {}

void Client::start(tcp::resolver::iterator endpoint_iter)
  {
    // Start the connect actor.
         start_connect(endpoint_iter);
  }

void Client::stop()
  {
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
      	        start_read();
               // start_write();
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
     //Start an asynchronous operation to send a heartbeat message.
           boost::asio::async_write(socket_, boost::asio::buffer("\n", 1),
           boost::bind(&Client::handle_write, this, _1));
}

void Client::handle_write(const boost::system::error_code& ec) {
	if(!ec) {
		std::string line; 
		std::getline(std::cin, line);
		start_write();
	} else {
		std::cout << ec.message() << "\n";
		stop();
	}
}

/*
void Client::run() {
	try{	//query for endpoints matching a server on specific port and ip
		 
	} catch (boost::system::system_error& e) {
		std::cerr << e.what() << std::endl;
	}	
}
*/
/* FOR USE AFTER INITIAL TESTS WRITTEN

May be better to just have listen() and send() which run Two infinite looping threads

void Client::readUserInput() {
	//user enters text until enter pressed	
	string_ptr message{new std::string{}};
	getline(std::cin, *message);
	//put message into queue
	(*messagesOut).push(message);	
}				

void Client::send(tcp::socket& socket) {
	//get next message to send
	std::string message = *(messagesOut->front());
	messagesOut->pop();
	boost::system::error_code error;
	//write message to socket
	size_t length = socket.write_some(buffer(message), error);
	//writeu until error set as EOF or actual system error
        if(error == boost::asio::error::eof) {
		std::cout << message << std::endl;
		return;	
	}
        else if (error)
	        throw boost::system::system_error(error);
	
}

void Client::readNewMessage(tcp::socket& socket) {
	for(;;) {
		//read data from server via socket into buffer
		boost::array<char, BUFFER_SIZE> buf;
		boost::system::error_code error;
		size_t length = socket.read_some(buffer(buf), error);
		//read until error set as EOF or actual system error
		if(error == boost::asio::error::eof)
			break;
		else if (error)
			throw boost::system::system_error(error);
		//write message to client's terminal
        	std::cout.write(buf.data(), length);
	}
}
*/


int main() {
	try {
    		boost::asio::io_service io_service;
    		tcp::resolver r(io_service);
    		Client c(io_service);

    		c.start(r.resolve(tcp::resolver::query(IP, PORT)));

    		io_service.run();
  	}
  	catch (std::exception& e)
  	{
    		std::cerr << "Exception: " << e.what() << "\n";
  	}

  	return 0;
}

