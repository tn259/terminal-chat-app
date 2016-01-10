#ifdef CLIENT_H	
#define CLIENT_H

/*
To begin with client contains a socket which just connects to server.
TODO Understand how to setup up boost socket so that connection to ruby server can be made.
*/

#include <iostream>
#include <string> 
#include <queue>

//Socket and network programming libraries
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

//namespace aliasing to enhance readability
//Note: must be sure that namespaces do not clash 
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

//typedef aliases to enhance readability
//Use boost smartpointers
typedef boost::shared_ptr<tcp::socket> socket_ptr; //perhaps use later
typedef boost::shared_ptr<std::string> string_ptr;
//All clients have access to the same message queue
typedef boost::shared_ptr< std::queue<string_ptr> > messageQueue_ptr;

class Client {
private:
	static const std::string IP = "127.0.0.1";
	static const int PORT = 3000;
	messageQueue_ptr messages;	
	void run();
public:
	Client();
};

#endif
