#ifndef CLIENT_H	
#define CLIENT_H

/*
To begin with client contains a socket which just connects to server.
TODO Understand how to setup up boost socket so that connection to ruby server can be made.
*/

// TODO: only <string>, <queue>, and <boost/shared_ptr.hpp> headers are needed here. The rest are implementation
//       details and should be hidden in the cpp file.

#include <iostream>
#include <string> 
#include <queue>

//Socket and network programming libraries
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>

// TODO: dont do this in the header as others who include it will have all of these
// identifiers pulled into their namespace too.

//namespace aliasing to enhance readability
//Note: must be sure that namespaces do not clash 
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

// TODO: move the socket def'n to the cpp as not needed here.
//typedef aliases to enhance readability
//Use boost smartpointers
typedef boost::shared_ptr<tcp::socket> socket_ptr; //perhaps use later
typedef boost::shared_ptr<std::string> string_ptr;
//All clients have access to the same message queue later for multiple clients
// TODO: is this queue really to be shared? if not make it a unique_ptr (part of C++11)
//       Note: shared_ptr is in std for c++11 too.
typedef boost::shared_ptr< std::queue<string_ptr> > messageQueue_ptr;

class Client {
private:
	// TODO: These two variables would be better defined in an anonymous namespace in the cpp file
	//       as they are an implementation detail.
	static const std::string IP;
	static const std::string PORT;
	messageQueue_ptr messages;	
public:
	Client();
	void run();
};

#endif
