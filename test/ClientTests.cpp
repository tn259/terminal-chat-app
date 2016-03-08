#include <gtest/gtest.h>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "../include/SocketHandler.hpp"
#include "ClientStub.hpp"
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
		boost::thread st(boost::bind(&boost::asio::io_service::run, io_ptr));
	//set up socket handler thread	
		io_ptrSH = std::shared_ptr<io_service>{new io_service};
		tcp::resolver resolverSH{*io_ptrSH};		
                tcp::resolver::query query{IP, PORT};
                tcp::resolver::iterator ep = resolverSH.resolve(query);
		tcp::socket socket{*io_ptrSH};
		sh1 = std::unique_ptr<SocketHandler>{new SocketHandler{socket, ep}};
		boost::thread sht(boost::bind(&boost::asio::io_service::run, io_ptrSH));
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	//set up client	
		io_ptrC = std::shared_ptr<io_service>{new io_service};
		tcp::resolver resolverC{*io_ptrC};	
		c1 = std::shared_ptr<ClientStub>{new ClientStub{*io_ptrC}};
    		c1->start(resolverC.resolve(tcp::resolver::query(IP, PORT)));
		boost::thread ct(boost::bind(&boost::asio::io_service::run, io_ptrC));
    		//io_service.run();
		//t.join();
			
	}
	virtual void TearDown() {
		io_ptrSH->stop();
		io_ptrC->stop();
		io_ptr->stop();
	}
	std::unique_ptr<SocketHandler> sh1;
	std::shared_ptr<ClientStub> c1;
	std::shared_ptr<io_service> io_ptr;
	std::shared_ptr<io_service> io_ptrSH;
	std::shared_ptr<io_service> io_ptrC;	
};

//TESTS
TEST_F(ClientTest, NoError) {
	ASSERT_NO_THROW(sh1->getMessages());
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


