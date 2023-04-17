#include "../include/Reader.h"
#include <vector>
#include <exception>
#include <signal.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
Reader::Reader(StompProtocol &protocol, ConnectionHandler &ch) : protocol(protocol), ch(ch){}

void Reader::run()
{
    while (!protocol.shouldTerminate)
    {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);

        std::string line(buf);
        if (protocol.isSummary(line))
        {
            std::cout << protocol.summary(line);
            continue;
        }
        std::vector<string> lines = protocol.processUser(line);

        for (string line : lines)
        {
            int len = line.length();
            if (!ch.sendLine(line)){
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }
            std::cout << "Sent " << len + 1 << " bytes to server" << std::endl;
        }
    }
}