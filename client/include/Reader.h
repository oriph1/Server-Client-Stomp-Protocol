#pragma once

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "StompProtocol.h"

using boost::asio::ip::tcp;

class Reader{
private:
    StompProtocol& protocol;
    ConnectionHandler& ch;
    
  
public:
    Reader(StompProtocol& protocol,ConnectionHandler& ch);
    void run();
};