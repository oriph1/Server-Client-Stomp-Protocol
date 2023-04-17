#pragma once
#include <string>
using std::string;
#include <map>
#include "Event.h"
class Summary
{
private:
    string gameName;
    string teamAname;
    string teamBname;
    std::map<string,string> generalStats;
    std::map<string,string> teamAstats;
    std::map<string,string> teamBstats;
    std::map<string,string> reports; //time - report

public:
    Summary();
    void addEvent(string,string gameName);
    string toString(string file);
    void addGeneralStats(std::map<string,string>);
    void addTeamStats(std::map<string,string> map,string team);
    string toStringHashMap(std::map<string,string> h);
    string toStringReports(std::map<string,string> h);

    void addreports(string discription,string time);
    void addEvent(Event event);
};

