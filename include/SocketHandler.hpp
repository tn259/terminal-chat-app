#ifndef SOCKETHANDLER_H        
#define SOCKETHANDLER_H

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>


/*boost tcp socket handler*/

class SocketHandler {
private:
        boost::asio::ip::tcp::socket soc;
public:
        SocketHandler(boost::asio::ip::tcp::socket&, boost::asio::ip::tcp::resolver::iterator&);
        void getMessages();
};

#endif
