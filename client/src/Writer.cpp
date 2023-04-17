#include "../include/Writer.h"
#include <vector>
#include <exception>
#include <signal.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
Writer::Writer(StompProtocol &protocol, ConnectionHandler &connectionHandler) : protocol(protocol), connectionHandler(connectionHandler) {}
// Reader::Reader(StompProtocol &protocol, ConnectionHandler &ch) : protocol(protocol), ch(ch), flagError(false) {}

void Writer::run()
{
    while (!protocol.shouldTerminate)
    {
        std::string answer;
        if (!connectionHandler.getLine(answer))
        {
            std::cout << "main theread is dead";
            std::cout << "Disconnected. Exiting...\n"
                      << std::endl;
            break;
        }

        answer = protocol.processServer(answer);
        if (answer != "")
        {
            std::cout << " \n" << answer << "\n ";
        }
        if (answer.find("Error") != std::string::npos){
            protocol.setUserName();
            std::cout << "Exiting... you need to login again \n"
                      << std::endl; 
            
        }

        if (answer.find("logout") != std::string::npos)
            std::cout << "time to say goodbye.....\n";
    }
}