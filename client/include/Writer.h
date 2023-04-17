#pragma once

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "StompProtocol.h"

using boost::asio::ip::tcp;

class Writer{
private:
    StompProtocol& protocol;
    ConnectionHandler& connectionHandler;

public:
    Writer(StompProtocol& protocol,ConnectionHandler& ch);
    void run();
};