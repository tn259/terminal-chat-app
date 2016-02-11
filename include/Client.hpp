#ifndef CLIENT_H	
#define CLIENT_H

/*
1. To begin with client contains a socket which just connects to server.
2. Send messages to the server
*/
#include <string> 
#include <queue>
#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>

//Use boost smartpointers
typedef std::shared_ptr<std::string> string_ptr;
//All clients have access to the same message queue later for multiple clients
typedef std::unique_ptr< std::queue<string_ptr> > messageQueue_ptr;

using namespace boost::asio::ip;
//using namespace boost::asio::ip::tcp;

class Client {
private:
	messageQueue_ptr messagesOut;
	messageQueue_ptr messagesIn;
	tcp::socket socket_;
	boost::asio::streambuf input_buffer_;
/*possible future methods for 2. all to be called in run
	void readUserInput();
	void send(boost::asio::ip::tcp::socket&);
	void readNewMessage(boost::asio::ip::tcp::socket&);
*/
	void stop();
	void start_connect(tcp::resolver::iterator endpoint_iter);
	void handle_connect(const boost::system::error_code& ec,
      tcp::resolver::iterator endpoint_iter);
	void start_read();
	void handle_read(const boost::system::error_code& ec);
	void start_write();
	void handle_write(const boost::system::error_code& ec);		
public:
	Client(boost::asio::io_service& io_service);
	//void run();	
	void start(tcp::resolver::iterator endpoint_iter);
};

#endif
