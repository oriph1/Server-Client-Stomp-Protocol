#include "StompProtocol.h"
#include <sstream>
#include <map>

StompProtocol::StompProtocol() : subCounter(0), recieptCounter(0),userName(""), subScribed(),summaries(),recieptMap(), shouldTerminate(false) {}

vector<string> StompProtocol::processUser(string s)
{
    stringstream input_stringstream(s);
    string word;
    getline(input_stringstream, word, ' ');
    vector<string> ans;
    if (word == "login")
    {
        if (userName != "")
        {
            std::cout << "The client is already logged in, log out before trying again";
            return ans;
        }
    }
    else if (word == "join")
    {
        string topic;
        getline(input_stringstream, topic, ' ');
        std::map<string, string> map;
        map["destination"] = topic;
        map["id"] = to_string(subCounter);
        subScribed[topic] = to_string(subCounter);
        subCounter++;
        map["reciept"] = to_string(recieptCounter);
        recieptMap[to_string(recieptCounter)] = "Joined channel " + topic;
        recieptCounter++;
        ans.push_back(transmit("SUBSCRIBE", map, ""));
        return ans;
    }
    else if (word == "exit")
    {
        string topic;
        getline(input_stringstream, topic, ' ');
        std::map<string, string> map;
        if (subScribed.count(topic) == 0)
        {
            shouldTerminate = true;
            map["id"] = "-1";
        }
        else
            map["id"] = subScribed[topic];
        map["reciept"] = to_string(recieptCounter);
        recieptMap[to_string(recieptCounter)] = "exit " + topic;
        recieptCounter++;
        subScribed.erase(topic);
        ans.push_back(transmit("UNSUBSCRIBE", map, ""));
        return ans;
    }
    else if (word == "report")
    {
        string file;
        getline(input_stringstream, file, ' ');
        file = "./data/" + file;
        names_and_events parsed = parseEventsFile(file);
        addEvents(parsed);
        ans = createVector(parsed);
        return ans;
    }
    else if (word == "logout")
    {
        ans.push_back(logoutFrame());
        return ans;
    }
    return ans;
}
string StompProtocol::logoutFrame()
{
    logout();
    std::map<string, string> map;
    map["reciept"] = to_string(recieptCounter);
    recieptMap[to_string(recieptCounter)] = "logout";
    return transmit("DISCONNECT", map, "");
}
void StompProtocol::logout()
{
    shouldTerminate = true;
    subScribed.clear();
    subCounter = 0;
    recieptCounter = 0;
    userName = "";
}
void StompProtocol::processLogIn(string s, ConnectionHandler &connectionHandler)
{
    shouldTerminate = false;
    string ans;
    stringstream input_stringstream(s);
    string word;
    getline(input_stringstream, word, ' ');
    string host;
    getline(input_stringstream, host, ' ');
    string userNameStr;
    getline(input_stringstream, userNameStr, ' ');
    string password;
    getline(input_stringstream, password, ' ');
    std::map<string, string> map;
    map["accept-version"] = "1.2";
    map["host"] = "stomp.cs.bgu.ac.il";
    map["login"] = userNameStr;
    map["passcode"] = password;
    ans += transmit("CONNECT", map, "");
    userName = userNameStr;
    connectionHandler.sendLine(ans);
}

string StompProtocol::summary(string s)
{
    string gameName;
    stringstream input_stringstream(s);
    getline(input_stringstream, gameName, ' ');
    getline(input_stringstream, gameName, ' '); // twice to skip "summary"
    string userName;
    getline(input_stringstream, userName, ' ');
    string file;
    getline(input_stringstream, file, ' ');
    // need to check if  the user and the game exist ###
    return (summaries[gameName])[userName].toString(file);
}
bool StompProtocol::isSummary(string s)
{
    stringstream input_stringstream(s);
    string word;
    getline(input_stringstream, word, ' ');
    return word == "summary";
}
void StompProtocol::addEvents(names_and_events parsed)
{
    for (Event e : parsed.events)
    {
        string gameName = e.get_team_a_name() + "_" + e.get_team_b_name();
        checksBeforeAddEvent(userName, gameName);
        if (subScribed.count(gameName) == 0)
        {
            shouldTerminate = true;
            break;
        }
        (summaries[gameName])[userName].addEvent(e); // add event to the summary of the user
    }
}
void StompProtocol::checksBeforeAddEvent(string userName, string gameName)
{
    if (summariesNotContainGame(gameName))
    {
        addGameToSummaries(gameName);
        addUserNameToGameSummary(userName, gameName);
    }
    else if (GameSummaryNotContainsUser(userName, gameName))
    {
        addUserNameToGameSummary(userName, gameName);
    }
}
bool StompProtocol::GameSummaryNotContainsUser(string userName, string gameName)
{
    return (summaries[gameName]).count(userName) == 0;
}
bool StompProtocol::summariesNotContainGame(string gameName)
{
    return summaries.count(gameName) == 0;
}
void StompProtocol::addGameToSummaries(string gameName)
{
    std::map<std::string, Summary> map; // create new map for the game
    summaries[gameName] = map;          // add to summaries the map
}
void StompProtocol::addUserNameToGameSummary(string userName, string gameName)
{
    Summary s;                           // create summay for the user that reported
    (summaries[gameName])[userName] = s; // add the summary to the user in the map
}
vector<string> StompProtocol::createVector(names_and_events parsed)
{
    vector<string> ans;
    for (Event e : parsed.events)
    {
        ans.push_back(createFrameEvent(e));
    }
    return ans;
}
string StompProtocol::createFrameEvent(Event e)
{
    string command = "SEND";
    string topic = e.get_team_a_name() + "_" + e.get_team_b_name();
    map<string, string> frameMap; // the frame headers
    frameMap["destination"] = topic;
    frameMap["reciept"] = to_string(recieptCounter);
    recieptMap[to_string(recieptCounter)] = "sent";
    recieptCounter++;
    string b = "";
    map<string, string> map;
    b += "user:" + userName + "\n";
    map["team a"] = e.get_team_a_name();
    map["team b"] = e.get_team_b_name();
    map["event name"] = e.get_name();
    map["time"] = to_string(e.get_time());
    b += hashMapToString(map);
    b += "general Game updates: \n";
    b += hashMapToString(e.get_game_updates());
    b += "team a updates: \n";
    b += hashMapToString(e.get_team_a_updates());
    b += "team b updates: \n";
    b += hashMapToString(e.get_team_b_updates());
    b += "description: \n";
    b += e.get_discription();
    return transmit(command, frameMap, b);
}
string StompProtocol::hashMapToString(std::map<string, string> h)
{
    string message = "";
    std::map<string, string>::iterator it = h.begin();
    while (it != h.end())
    {
        message += it->first;
        message += (":");
        message += it->second;
        message += ("\n");
        it++;
    }
    return message;
}
string StompProtocol::transmit(string c, std::map<string, string> h, string b)
{
    string message(c);
    message += ("\n");
    std::map<string, string>::iterator it = h.begin();
    while (it != h.end())
    {
        message += it->first;
        message += (":");
        message += it->second;
        message += ("\n");
        it++;
    }
    message += ("\n");
    message += (b);
    return message;
}

string StompProtocol::processServer(string s)
{
    string command;
    stringstream input_stringstream(s);
    getline(input_stringstream, command, '\n');
    if (command == "CONNECTED")
        return "Login successful";
    else if (command == "ERROR")
        return ERROR(s);
    else if (command == "MESSAGE")
        return MESSEGE(s);
    else if (command == "RECIEPT")
        return RECIEPT(s);
    return "";
}

string StompProtocol::ERROR(string s)
{
    shouldTerminate=true;
    if ((s.find("Wrong password") != std::string::npos))
        return "Error: Wrong password";
    else if ((s.find("User already logged in") != std::string::npos))
        return "Error: User already logged in";
    return "Error: need to join the channel first";
}
string StompProtocol::MESSEGE(string s)
{
    stringstream messege_Stream(s);
    std::deque<std::string> words;
    std::string line;
    while (getline(messege_Stream, line, '\n'))
    {
        words.push_back(line);
    }
    while (!(((words.front()).find("user")) != std::string::npos))
    {
        words.pop_front();
    }
    string user = get_user_name(words);
    Event bs(words);
    addingEvent(bs, user);
    return "";
}

void StompProtocol::addingEvent(Event &e, string userid)
{
    string gameName = e.get_team_a_name() + "_" + e.get_team_b_name();
    checksBeforeAddEvent(userid, gameName);
    (summaries[gameName])[userid].addEvent(e); // add event to the summary of the user
}

string StompProtocol::get_user_name(deque<std::string> &s)
{ // get the user name from deque
    string user_line = s.front();
    int index = user_line.find(":");
    string user_name = user_line.substr(index + 1);
    s.pop_front(); // remove user name
    return user_name;
}

string StompProtocol::RECIEPT(string s)
{
    stringstream input_stringstream(s);
    string tempString;
    getline(input_stringstream, tempString, '\n');
    getline(input_stringstream, tempString, '\n');
    int index = tempString.find(":");
    tempString = tempString.substr(index + 1);
    return recieptMap[tempString];
}

void StompProtocol::setUserName(){
userName = "";
}