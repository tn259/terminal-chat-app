#ifndef SERVER_H
#define SERVER_H

//Refactored from Kohloff

// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>

std::string make_daytime_string();

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
public:
	typedef boost::shared_ptr<tcp_connection> pointer;
	
	static pointer create(boost::asio::io_service& io_service)
	{
	    return tcp_connection::pointer(new tcp_connection(io_service));
  	}

	boost::asio::ip::tcp::socket& socket();
	boost::asio::streambuf& streambuf();
	void start();
private:
	tcp_connection(boost::asio::io_service& io_service);
	void handle_write(const boost::system::error_code&, size_t);

	boost::asio::ip::tcp::socket socket_;
	boost::asio::streambuf buf_;
	std::string message_;
};
	
class tcp_server
{
public:
 	tcp_server(boost::asio::io_service& io_service);
private:
  	void start_accept();
	void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error);
	void listen_and_broadcast(tcp_connection::pointer&);
	void broadcast(const boost::system::error_code&, tcp_connection::pointer&);
	boost::asio::ip::tcp::acceptor acceptor_;
	std::vector<tcp_connection::pointer> connected_clients;	
};

#endif
