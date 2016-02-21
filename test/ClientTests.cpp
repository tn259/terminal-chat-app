#include <gtest/gtest.h>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "../include/SocketHandler.hpp"
#include "../include/Client.hpp"
#include "server.hpp"

/*
Check that each method does not throw system_error
Check that the buffer does not overflow
Implement using SocketHandler
Start by creating a test fixture for this
For now we will treat ClientTests.cpp as the main executable test file
*/

/*
TODO From setting up this first test:
Must make sure I understand asio better
Brush up on Multithreading and particularly as client of std::thread and boost::thread
Learn to mock Kohlhoff's server example in gmock
NEXT: Sending and receiving messages, need to think about design carefully before proceeding with this, e.g. SocketHandler maybe better replaced with ios handler..? etc. start with messages modelled simply as std::string. 

Will need to update these tests after adding send and receive messages feature
*/

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace {
        const std::string IP = "127.0.0.1";
        const std::string PORT = "3000";
}



//test fixture
class ClientTest : public testing::Test {
protected:
	virtual void SetUp() {
	//set up server thread
		io_ptr = std::shared_ptr<io_service>{new io_service};
    		tcp_server server(*io_ptr);
		boost::thread bt(boost::bind(&boost::asio::io_service::run, io_ptr));
	//set up socket handler thread	
		io_ptr2 = std::shared_ptr<io_service>{new io_service};
		tcp::resolver resolver{*io_ptr2};
		Client c{*io_ptr2};
    		c.start(resolver.resolve(tcp::resolver::query(IP, PORT)));	
		boost::thread t(boost::bind(&boost::asio::io_service::run, io_ptr2));
    		io_ptr->run();
		t.join();
	
                /*tcp::resolver::query query{IP, PORT};
                tcp::resolver::iterator ep = resolver.resolve(query);
		tcp::socket socket{*io_ptr2};
		sh1 = std::unique_ptr<SocketHandler>{new SocketHandler{socket, ep}};*/
		boost::thread bt2(boost::bind(&boost::asio::io_service::run, io_ptr2));
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));	
	}
	virtual void TearDown() {
		io_ptr2->stop();
		io_ptr->stop();
	}
	std::unique_ptr<SocketHandler> sh1;
	std::shared_ptr<io_service> io_ptr;
	std::shared_ptr<io_service> io_ptr2;	
};

//TESTS
TEST_F(ClientTest, NoError) {
	ASSERT_NO_THROW(sh1->getMessages());
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


