#ifndef CLIENT_H	
#define CLIENT_H

/*
To begin with client contains a socket which just connects to server.
*/

#include <iostream>
#include <string> 
#include <queue>

//Socket and network programming libraries
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>
//Use boost smartpointers
typedef boost::shared_ptr<tcp::socket> socket_ptr; //perhaps use later
typedef boost::shared_ptr<std::string> string_ptr;
//All clients have access to the same message queue later for multiple clients
typedef boost::shared_ptr< std::queue<string_ptr> > messageQueue_ptr;

class Client {
private:
	static const std::string IP;
	static const std::string PORT;
	messageQueue_ptr messages;	
public:
	Client();
	void run();
};

#endif
