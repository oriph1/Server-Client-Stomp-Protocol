#pragma once

#include "../include/ConnectionHandler.h"
using std::string;
// TODO: implement the STOMP protocol
#include <map>
#include "Event.h"
#include <vector>
using namespace std;
class Summary;
#include "Summary.h"
#include <deque>

class StompProtocol
{
private:
 int subCounter;
 int recieptCounter;
 std::string userName;
 std::map<string,string> subScribed; //topic - id
 std::map<string,std::map<string,Summary>> summaries;
 std::map<string,string> recieptMap; //reciept - action

public:
bool shouldTerminate;
 StompProtocol();
 void setUserName();
 vector<string> processUser(string);
 void processLogIn(string s, ConnectionHandler &connectionHandler);
 string processServer(string);
 string transmit(string c,std::map<string,string> h,string b);
 string hashMapToString(std::map<string,string> h);
 string createFrameEvent(Event e);
 vector<string> createVector(names_and_events parsed);
 void addEvents(names_and_events parsed);
 bool isSummary(string s);
 string summary(string s);
 void  send(std::vector<string> lines);
 bool logIn(string s);
string CONNECTED(string s);
string  ERROR(string s);
string  MESSEGE(string s);
string  RECIEPT(string s);
std::map<std::string, std::string> mappify2(std::string const& s);
bool GameSummaryNotContainsUser(string userName,string gameName);
bool summariesNotContainGame(string gameName);
void addGameToSummaries(string gameName);
void addUserNameToGameSummary(string userName,string gameName);
void checksBeforeAddEvent(string userName,string gameName);
string logoutFrame();

//for the event constructure from procces
string get_user_name(std::deque<std::string> & s);
void addingEvent(Event &e, string userid);
void logout();
};
