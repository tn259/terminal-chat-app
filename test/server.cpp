//Refactored and modified from Kohloff

// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/******
WARNING UNFUNCTIONING CODE
****/

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "server.hpp"

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}


tcp::socket& tcp_connection::socket()
  {
    return socket_;
  }

boost::asio::streambuf& tcp_connection::streambuf() {
	return buf_;
}

void tcp_connection::start()
  {
    message_ = make_daytime_string();

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

tcp_connection::tcp_connection(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
  }

void tcp_connection::handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }


tcp_server::tcp_server(boost::asio::io_service& io_service)
    : acceptor_(io_service, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 3000)),
	connected_clients{}
  {
    start_accept();
  }

void tcp_server::start_accept()
  {
    tcp_connection::pointer new_connection =
      tcp_connection::create(acceptor_.get_io_service());
	std::cout << "Server start async accept\n";
    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }

void tcp_server::handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error)
  {
	std::cout << "Server handling connection\n";
    if (!error)
    {
     // new_connection->start();
	std::cout << "Server no error in handling socket\n";
	connected_clients.push_back(new_connection);
	std::cout << "Server added new connection\n";
	boost::asio::strand strand{new_connection->socket().get_io_service()};
	strand.post(strand.wrap(boost::bind(&tcp_server::listen_and_broadcast, this, new_connection)));
	boost::this_thread::sleep(boost::posix_time::milliseconds(100));	
      start_accept();
    }
  }

void tcp_server::listen_and_broadcast(tcp_connection::pointer& connection) {
	std::cout << "Server listening and broadcasting\n";
	boost::asio::async_read_until(connection->socket(), connection->streambuf(), "\n",
		boost::bind(&tcp_server::broadcast, this, boost::asio::placeholders::error,
		 connection));
}

void tcp_server::broadcast(const boost::system::error_code& ec, tcp_connection::pointer& connection) {
	if(!ec) {
		std::cout << "Server broadcasting" << "\n";
		std::string message;
		std::istream is(&(connection->streambuf()));
		std::getline(is, message);
		std::cout << "Server read message: " << message << "\n";
		for(auto con : connected_clients)
			boost::asio::write(con->socket(), boost::asio::buffer(message));
	}
}			
	
	
