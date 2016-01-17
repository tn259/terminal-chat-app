#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <utility>
#include "Client.hpp"

//namespace aliasing to enhance readability
////Note: must be sure that namespaces do not clash 
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
//constants in anon namespace good for encapsulation
namespace {
	const std::string IP = "127.0.0.1";
	const std::string PORT = "3000";
	const size_t BUFFER_SIZE = 128;
}

Client::Client(): messagesOut{}, messagesIn{} {}

void Client::run() {
	try{	//query for endpoints matching a server on specific port and ip
		io_service io_s;
		tcp::resolver resolver{io_s};
		tcp::resolver::query query{IP, PORT};
		tcp::resolver::iterator endpoint = resolver.resolve(query);
		
		tcp::socket socket{io_s};
		connect(socket, endpoint); //throws error if no connection

		for(;;) {
			//read data from server via socket into buffer
			boost::array<char, 128> buf;
			boost::system::error_code error;
			size_t length = socket.read_some(buffer(buf), error);
			//read until error set as EOF or actual system error
			if(error == boost::asio::error::eof)
				break;
			else if (error)
				throw boost::system::system_error(error);
			//signify about to write message to terminal
			std::cout << "-->" << std::endl;
			std::cout.write(buf.data(), length);	
		}
		 
	} catch (boost::system::system_error& e) {
		std::cerr << e.what() << std::endl;
	}	
}

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
	Client c{};
	c.run();
	return 0;
}
