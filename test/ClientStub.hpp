#ifndef CLIENTSTUB_H	
#define CLIENTSTUB_H

/*
ClientStub now capable of reading and writing messages to server
*/
#include <string> 
#include <queue>
#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>

//Maybe use boost smartpointers for message storing later
typedef std::unique_ptr< std::queue<std::string> > messageQueue_ptr;

using namespace boost::asio::ip;

class ClientStub {
private:
	tcp::socket socket_;
	boost::asio::streambuf input_buffer_;
	boost::asio::strand strand_read;
	boost::asio::strand strand_write;
	messageQueue_ptr incomingMessages;
	messageQueue_ptr outgoingMessages;
	void stop();
	void start_connect(tcp::resolver::iterator endpoint_iter);
	void handle_connect(const boost::system::error_code& ec,
      tcp::resolver::iterator endpoint_iter);
	void start_read();
	void handle_read(const boost::system::error_code& ec);
	void start_write();
	void start_write(std::string&);
	void handle_write(const boost::system::error_code& ec);		
public:
	ClientStub(boost::asio::io_service& io_service);
	void start(tcp::resolver::iterator endpoint_iter);
	void strandRead();
	void strandWrite(std::string&);
	std::string lastIncoming();
};

#endif
