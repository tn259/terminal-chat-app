#include "../include/SocketHandler.hpp"
#include <iostream>

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace {
	const size_t BUFFER_SIZE = 64;
}

SocketHandler::SocketHandler(tcp::socket& socket, tcp::resolver::iterator& endpoint) : soc{std::move(socket)} {
        connect(soc, endpoint);
}

void SocketHandler::getMessages() {
        for(;;) {
                //read data from server via socket into buffer
                boost::array<char, BUFFER_SIZE> buf;
                boost::system::error_code error;
                size_t length = soc.read_some(buffer(buf), error);
                //read until error set as EOF or actual system error
                if(error == boost::asio::error::eof)
                        break;
                else if (error)
                        throw boost::system::system_error(error);
                //signify about to write message to terminal
                std::cout << "-->" << std::endl;
                std::cout.write(buf.data(), length);
        }
}
