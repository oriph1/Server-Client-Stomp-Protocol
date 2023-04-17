#include "../include/Summary.h"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
using namespace std;
Summary::Summary() : gameName(),teamAname(""),teamBname(""),generalStats(), teamAstats(), teamBstats(), reports()
{
}
void Summary::addEvent(Event event)
{
    if (gameName == "")
    {
        teamAname = event.get_team_a_name();
        teamBname = event.get_team_b_name();
        gameName = teamAname + "_" + teamBname;
    }
    addGeneralStats(event.get_game_updates());
    addTeamStats(event.get_team_a_updates(), "A");
    addTeamStats(event.get_team_b_updates(), "B");
    addreports(event.get_discription(), to_string(event.get_time()) + " - " + event.get_name());
}
void Summary::addEvent(string event, string gameName)
{
    // ofstream myfile;
    // myfile.open("example.json");
    // myfile << event;
    // myfile.close();
    // names_and_events parsed = parseEventsFile("example.json");
    // for (Event e : parsed.events)
    // {
    //     addEvent(e);
    // }
}
void Summary::addGeneralStats(std::map<string, string> map)
{
    std::map<string, string>::iterator it = map.begin();
    while (it != map.end())
    {
        generalStats[it->first] = it->second; // add the stats to the general updates
        it++;
    }
}
void Summary::addTeamStats(std::map<string, string> map, string team)
{
    std::map<string, string>::iterator it = map.begin();
    while (it != map.end())
    {
        if (team == "A")
            teamAstats[it->first] = it->second; // add the stats to the team A updates
        if (team == "B")
            teamBstats[it->first] = it->second; // add the stats to the team B updates
        it++;
    }
}
void Summary::addreports(string discription, string header)
{
    reports[header] = discription;
}
string Summary::toString(string file)
{
    string ans = "";
    ans += gameName + "\n";
    ans += "Game stats: \n";
    ans += "General stats: \n";
    ans += toStringHashMap(generalStats);
    ans += teamAname + " stats: \n";
    ans += toStringHashMap(teamAstats);
    ans += teamBname + " stats: \n";
    ans += toStringHashMap(teamBstats);
    ans += "Game event reports: \n";
    ans += toStringReports(reports);
    ofstream myfile;
    myfile.open(file);
    myfile << ans;
    myfile.close();
    return ans;
}
string Summary::toStringHashMap(std::map<string, string> h)
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
string Summary::toStringReports(std::map<string, string> h)
{
    string message = "";
    std::map<string, string>::iterator it = h.begin();
    while (it != h.end())
    {
        message += it->first;
        message += (":\n\n");
        message += it->second;
        message += ("\n\n\n");
        it++;
    }
    return message;
}
