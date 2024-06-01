#ifndef P_CONF_HPP
#define P_CONF_HPP
#include <fstream>
#include <windows.h>
#include "DataGroup.hpp"
#include "Pstr.hpp"

int GetStatu(const std::string &str);
int GetDay(const std::string &str);
int GetType(const std::string &str);
Startup GetStartup(const std::string &str);
std::vector<Rule> GetRule(const std::string &str);
std::vector<Plan> GetPlan(const std::string &str);

struct Config
{
    Startup startup;
    std::vector<Rule> rules;
    std::vector<Plan> plans;
};

Config GetConfig()
{
    // Getbasepath
    char szpath[512] = {0};
    GetModuleFileName(NULL, szpath, sizeof(szpath) - 1);
    std::string basepath = szpath;
    basepath = basepath.substr(0, basepath.rfind("\\") + 1);
    // Read
    Config config;
    std::ifstream rd(basepath + "rule.conf");
    std::string result;
    while (std::getline(rd, result))
    {
        if (result.find("Startup:") == 0)
            config.startup = GetStartup(result.substr(result.find(":") + 1));
        else if (result.find("Rule:") == 0)
            config.rules = GetRule(result.substr(result.find(":") + 1));
        else if (result.find("Plan:") == 0)
            config.plans = GetPlan(result.substr(result.find(":") + 1));
    }
    rd.close();
    return config;
}

Startup GetStartup(const std::string &str)
{
    Startup res;
    std::string buff;
    int statu = 0;
    for (auto &i : str)
    {
        if (i == '[' || i == ']')
            continue;
        else if (i == '{')
        {
            statu = GetStatu(SpaceClear(buff));
            buff.clear();
        }
        else if (i == '}' || i == ',')
        {
            if (statu == 1)
                res.command.emplace_back(SpaceClear(buff));
            else if (statu == 4)
                res.sleep = std::stoi(SpaceClear(buff));
            buff.clear();
            if (i == '}')
                statu = 0;
        }
        else
            buff += i;
    }
    return res;
}

std::vector<Rule> GetRule(const std::string &str)
{
    // Write
    std::vector<Rule> res;
    Rule current, forall;
    std::string buff;
    int statu = 0, cnt = 0;
    for (auto &i : str)
    {
        if (i == '[')
        {
            ++cnt;
            if (cnt == 2)
                current.clear();
        }
        else if (i == ']')
        {
            if (cnt == 2)
            {
                if (current.command.size() == 0)
                    current.command = forall.command;
                if (current.date.size() == 0)
                    current.date = forall.date;
                if (current.timeseg.size() == 0)
                    current.timeseg = forall.timeseg;
                if (current.sleep == -1)
                    current.sleep = forall.sleep;
                res.emplace_back(current);
            }
            --cnt;
        }
        else if (i == '{')
        {
            statu = GetStatu(SpaceClear(buff));
            buff.clear();
        }
        else if (i == '}' || i == ',')
        {
            switch (statu)
            {
            case 1:
            {
                if (cnt == 1)
                    forall.command.emplace_back(SpaceClear(buff));
                else
                    current.command.emplace_back(SpaceClear(buff));
                break;
            }
            case 2:
            {
                if (cnt == 1)
                    forall.date.emplace_back(GetDay(SpaceClear(buff)));
                else
                    current.date.emplace_back(GetDay(SpaceClear(buff)));
                break;
            }
            case 3:
            {
                if (cnt == 1)
                    forall.timeseg.emplace_back(ToTimeSeg(SpaceClear(buff)));
                else
                    current.timeseg.emplace_back(ToTimeSeg(SpaceClear(buff)));
                break;
            }
            case 4:
            {
                if (cnt == 1)
                    forall.sleep = std::stoi(SpaceClear(buff));
                else
                    current.sleep = std::stoi(SpaceClear(buff));
                break;
            }
            }
            if (i == '}')
                statu = 0;
            buff.clear();
        }
        else
        {
            buff += i;
        }
    }
    return res;
}

std::vector<Plan> GetPlan(const std::string &str){
    std::vector<Plan> res;
    return res;
}

int GetStatu(const std::string &str)
{
    std::string buff = SpaceClear(str);
    if (buff == "Command")
        return 1;
    else if (buff == "Date")
        return 2;
    else if (buff == "Time")
        return 3;
    else if (buff == "Sleep")
        return 4;
    return 0;
}

int GetType(const std::string &str)
{
    std::string buff = SpaceClear(str);
    if (buff == "Startup")
        return 1;
    else if (buff == "Rule")
        return 2;
    return 0;
}

int GetDay(const std::string &str)
{
    std::string buff = SpaceClear(str);
    std::vector<std::string> Groupstr = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    for (auto i = 0; i < Groupstr.size(); i++)
        if (buff == Groupstr[i])
            return i;
    return -1;
}

#endif