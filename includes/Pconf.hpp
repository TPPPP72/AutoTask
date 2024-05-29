#ifndef P_CONF_HPP
#define P_CONF_HPP
#include <fstream>
#include <windows.h>
#include "DataGroup.hpp"
#include "Pstr.hpp"

int GetStatu(const std::string &str);
int GetDay(const std::string &str);

std::vector<Rule> GetRules()
{
    // Getbasepath
    char szpath[512] = {0};
    GetModuleFileName(NULL, szpath, sizeof(szpath) - 1);
    std::string basepath = szpath;
    basepath = basepath.substr(0, basepath.rfind("\\") + 1);
    // Read
    std::ifstream rd(basepath + "rule.conf");
    std::string result;
    std::getline(rd, result);
    rd.close();
    // Write
    std::vector<Rule> res;
    Rule current, forall;
    std::string buff;
    int statu = 0, cnt = 0;
    for (auto &i : result)
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
                current.command = forall.command, res.emplace_back(current);
            --cnt;
        }
        else if (i == '{')
        {
            statu = GetStatu(buff);
            buff.clear();
        }
        else if (i == '}' || i == ',')
        {
            switch (statu)
            {
            case 1:
            {
                if (cnt == 1)
                    forall.command.emplace_back(buff);
                else
                    current.command.emplace_back(buff);
                break;
            }
            case 2:
            {
                current.date.emplace_back(GetDay(buff));
                break;
            }
            case 3:
            {
                current.timeseg.emplace_back(ToTimeSeg(buff));
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

int GetStatu(const std::string &str)
{
    std::string buff = SpaceClear(str);
    if (buff == "Command")
        return 1;
    else if (buff == "Date")
        return 2;
    else if (buff == "Time")
        return 3;
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