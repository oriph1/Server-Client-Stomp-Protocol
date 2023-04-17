#include "../include/Event.h"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <deque>
using json = nlohmann::json;

Event::Event(std::string team_a_name, std::string team_b_name, std::string name, int time,
             std::map<std::string, std::string> game_updates, std::map<std::string, std::string> team_a_updates,
             std::map<std::string, std::string> team_b_updates, std::string discription)
    : team_a_name(team_a_name), team_b_name(team_b_name), name(name),
      time(time), game_updates(game_updates), team_a_updates(team_a_updates),
      team_b_updates(team_b_updates), description(discription)
{
}

Event::~Event()
{
}

const std::string &Event::get_team_a_name() const
{
    return this->team_a_name;
}

const std::string &Event::get_team_b_name() const
{
    return this->team_b_name;
}

const std::string &Event::get_name() const
{
    return this->name;
}

int Event::get_time() const
{
    return this->time;
}

const std::map<std::string, std::string> &Event::get_game_updates() const
{
    return this->game_updates;
}

const std::map<std::string, std::string> &Event::get_team_a_updates() const
{
    return this->team_a_updates;
}

const std::map<std::string, std::string> &Event::get_team_b_updates() const
{
    return this->team_b_updates;
}

const std::string &Event::get_discription() const
{
    return this->description;
}

Event::Event(const std::string &frame_body) : team_a_name(""), team_b_name(""), name(""), time(0), game_updates(), team_a_updates(), team_b_updates(), description("")
{
}
Event::Event(std::deque<std::string> &spilted) : team_a_name(""), team_b_name(""), name(""), time(0), game_updates(), team_a_updates(), team_b_updates(), description("")
{
setEventName(spilted);
setTeamAName(spilted);
setTeamBName(spilted);
setTime(spilted);
set_General_Game_Updates(spilted);
set_Team_A_Updates(spilted);
set_Team_B_Updates(spilted);
set_discription(spilted);
}
void Event::setEventName(std::deque<std::string> &spilted){
std::string user_line = spilted.front();
int index = user_line.find(":"); 
std::string event_name = user_line.substr(index+1);
name = event_name;
spilted.pop_front(); //remove event name
}
void Event::setTeamAName(std::deque<std::string> &spilted){
std::string user_line = spilted.front();
int index = user_line.find(":"); 
std::string teamName = user_line.substr(index+1);
team_a_name = teamName;
spilted.pop_front(); //remove TeamAName name

}
void Event::setTeamBName(std::deque<std::string> &spilted){
std::string user_line = spilted.front();
int index = user_line.find(":"); 
std::string teamName = user_line.substr(index+1);
team_b_name = teamName;
spilted.pop_front(); //remove TeamBName name

}
void Event::setTime(std::deque<std::string> &spilted){
std::string user_line = spilted.front();
int index = user_line.find(":"); 
std::string tim = user_line.substr(index+1);
int c = stoi(tim);
time = c;
spilted.pop_front(); //remove time
}
    void Event::set_General_Game_Updates(std::deque<std::string> &spilted){
        std::string user_line = spilted.front();
        while (!(user_line.find("team a updates") != std::string::npos)){
            if (user_line.find("general game") != std::string::npos){
                spilted.pop_front();
                user_line = spilted.front();
                continue;
            }
            else{
                int index = user_line.find(":");
                game_updates[(user_line.substr(0,index))] = (user_line.substr(index + 1));
                spilted.pop_front();
                user_line = spilted.front(); 
            }
        }
    }
    void Event::set_Team_A_Updates(std::deque<std::string> &spilted){
        std::string user_line = spilted.front();
        while (!(user_line.find("team b updates") != std::string::npos)){
              if (user_line.find("team a updates") != std::string::npos){
                spilted.pop_front();
                user_line = spilted.front();
                continue;
            }
            else{
                int index = user_line.find(":");
                team_a_updates[(user_line.substr(0,index))] = (user_line.substr(index + 1));
                spilted.pop_front();
                user_line = spilted.front();  
            }
        }
    }
    void Event::set_Team_B_Updates(std::deque<std::string> &spilted){
        std::string user_line = spilted.front();
        while (!(user_line.find("description") != std::string::npos)){
                if (user_line.find("team b updates") != std::string::npos){
                    spilted.pop_front();
                    user_line = spilted.front();
                    continue;
                }
                else{
                    int index = user_line.find(":");
                    team_b_updates[(user_line.substr(0,index))] = (user_line.substr(index + 1));
                    spilted.pop_front();
                    user_line = spilted.front();  
                }    
        }
    }
    void Event::set_discription(std::deque<std::string> &spilted){
        std::string user_line = spilted.front();
        if (user_line.find("description") != std::string::npos){
            spilted.pop_front();
            user_line = spilted.front();
            description = user_line;
        }
    }

names_and_events parseEventsFile(std::string json_path)
{   
    std::ifstream f(json_path);
    json data = json::parse(f);
    //std::cout<<"parsed";
    std::string team_a_name = data["team a"];
    std::string team_b_name = data["team b"];

    // run over all the events and convert them to Event objects
    std::vector<Event> events;
    for (auto &event : data["events"])
    {
        std::string name = event["event name"];
        int time = event["time"];
        std::string description = event["description"];
        std::map<std::string, std::string> game_updates;
        std::map<std::string, std::string> team_a_updates;
        std::map<std::string, std::string> team_b_updates;
        for (auto &update : event["general game updates"].items())
        {
            if (update.value().is_string())
                game_updates[update.key()] = update.value();
            else
                game_updates[update.key()] = update.value().dump();
        }

        for (auto &update : event["team a updates"].items())
        {
            if (update.value().is_string())
                team_a_updates[update.key()] = update.value();
            else
                team_a_updates[update.key()] = update.value().dump();
        }

        for (auto &update : event["team b updates"].items())
        {
            if (update.value().is_string())
                team_b_updates[update.key()] = update.value();
            else
                team_b_updates[update.key()] = update.value().dump();
        }
        
        events.push_back(Event(team_a_name, team_b_name, name, time, game_updates, team_a_updates, team_b_updates, description));
    }
    names_and_events events_and_names{team_a_name, team_b_name, events};

    return events_and_names;
}