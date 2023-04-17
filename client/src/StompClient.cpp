#include <stdlib.h>
#include "../include/ConnectionHandler.h"
#include "../include/StompProtocol.h"
#include "../include/Reader.h"
#include "../include/Writer.h"
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>
#include <mutex>
/**
 * This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
 */
void login(string &host, string &port, string &lines);
int main(int argc, char *argv[])
{
    StompProtocol protocol;
    while (1)
    {
        std::string host;  // the host of login command
        std::string port;  // the port of login command
        std::string lines; // the login command for sending frame and connect as a user
        login(host,port,lines);
        short portShort = boost::lexical_cast<short>(port);
        ConnectionHandler connectionHandler(host, portShort);
        if (!connectionHandler.connect())
        {
            std::cerr << "Cannot connect to " << host << ":" << port << std::endl; 
            return 1;   
        }                                                         
        protocol.processLogIn(lines, connectionHandler);  
        Reader reader(protocol, connectionHandler); 
        Writer writer(protocol, connectionHandler); 
        string answer;   
        connectionHandler.getLine(answer);   
        answer = protocol.processServer(answer);
        if (answer != "")
        {
            std::cout << " \n" << answer << "\n ";
        }
        if (answer.find("Error") != std::string::npos){
             protocol.logout();
             continue;
        }
        std::thread th2(&Writer::run, &writer);
        std::thread th1(&Reader::run, &reader);
        th1.join();
        th2.join();
        protocol.logout();
    }
}
void login(string &host, string &port, string &lines)
{
    while (1)
    {
        std::cout << "please login\n";
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        lines=line;
        stringstream input_stringstream(line);
        string word;
        getline(input_stringstream, word, ' ');
        if (word != "login")
        { /////////////if the first command is not login
            std::cout << "Need to login first";
            continue;
        }
        getline(input_stringstream, host, ':');
        getline(input_stringstream, port, ' ');
        break;
    }
}
