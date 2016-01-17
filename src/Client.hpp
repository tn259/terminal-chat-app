#ifndef CLIENT_H	
#define CLIENT_H

/*
1. To begin with client contains a socket which just connects to server.
2. Send messages to the server
*/

#include <string> 
#include <queue>
#include <boost/shared_ptr.hpp>
#include <memory>

//Use boost smartpointers
//typedef boost::shared_ptr<tcp::socket> socket_ptr; //perhaps use later
typedef std::shared_ptr<std::string> string_ptr;
//All clients have access to the same message queue later for multiple clients
typedef std::shared_ptr< std::queue<string_ptr> > messageQueue_ptr;

class Client {
private:
	messageQueue_ptr messages;
//possible future methods for 2. all to be called in run
	void readUserInput();
	void send();		
public:
	Client();
	void run();
};

#endif
