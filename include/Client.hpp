#ifndef CLIENT_H	
#define CLIENT_H

/*
1. To begin with client contains a socket which just connects to server.
2. Send messages to the server
*/
#include <string> 
#include <queue>
#include <memory>

//Use boost smartpointers
typedef std::shared_ptr<std::string> string_ptr;
//All clients have access to the same message queue later for multiple clients
typedef std::unique_ptr< std::queue<string_ptr> > messageQueue_ptr;


class Client {
private:
	messageQueue_ptr messagesOut;
	messageQueue_ptr messagesIn;
/*possible future methods for 2. all to be called in run
	void readUserInput();
	void send(boost::asio::ip::tcp::socket&);
	void readNewMessage(boost::asio::ip::tcp::socket&);
*/		
public:
	Client();
	void run();
};

#endif
