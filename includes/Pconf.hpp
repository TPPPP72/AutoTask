#ifndef P_CONF_HPP
#define P_CONF_HPP
#include <fstream>
#include "DataGroup.hpp"

std::vector<Rule> GetRules()
{
    // Read
    std::ifstream rd("rule.conf");
    std::string result;
    std::getline(rd, result);
    rd.close();
    // Write
    std::vector<Rule> r;
    std::vector<std::string> forallcommand;
    std::string buff;
    Rule rr;
    int statu = 0, cnt = 0;
    for (auto &i : result)
    {
        if (isalnum(i) || i == '.' || i == '/' || i == '-' || i == ' ' || i == ':')
        {
            buff += i;
        }
        else
        {
            if (i == '[')
            {
                ++cnt;
                if (cnt == 2)
                {
                    rr.command.clear();
                    rr.date.clear();
                    rr.timeseg.clear();
                }
            }
            else if (i == ']')
            {
                if (cnt == 2)
                {
                    rr.command = forallcommand;
                    r.emplace_back(rr);
                }
                --cnt;
            }
            else if (i == '{')
            {
                if (buff == "Command")
                    statu = 1;
                else if (buff == "Date")
                    statu = 2;
                else if (buff == "Time")
                    statu = 3;
            }
            else if (i == '}')
            {
                if (statu == 1)
                {
                    if (cnt == 1)
                        forallcommand.emplace_back(buff);
                    else
                        rr.command.emplace_back(buff);
                }
                else if (statu == 2)
                {
                    if (buff == "Mon")
                        rr.date.emplace_back(Mon);
                    if (buff == "Tue")
                        rr.date.emplace_back(Tue);
                    if (buff == "Wed")
                        rr.date.emplace_back(Wed);
                    if (buff == "Thu")
                        rr.date.emplace_back(Thu);
                    if (buff == "Fri")
                        rr.date.emplace_back(Fri);
                    if (buff == "Sat")
                        rr.date.emplace_back(Sat);
                    if (buff == "Sun")
                        rr.date.emplace_back(Sun);
                }
                else if (statu == 3)
                    rr.timeseg.emplace_back(ToTimeSeg(buff));
                statu = 0;
            }
            else
            {
                if (statu == 1)
                {
                    if (cnt == 1)
                        forallcommand.emplace_back(buff);
                    else
                        rr.command.emplace_back(buff);
                }
                else if (statu == 2)
                {
                    if (buff == "Mon")
                        rr.date.emplace_back(Mon);
                    if (buff == "Tue")
                        rr.date.emplace_back(Tue);
                    if (buff == "Wed")
                        rr.date.emplace_back(Wed);
                    if (buff == "Thu")
                        rr.date.emplace_back(Thu);
                    if (buff == "Fri")
                        rr.date.emplace_back(Fri);
                    if (buff == "Sat")
                        rr.date.emplace_back(Sat);
                    if (buff == "Sun")
                        rr.date.emplace_back(Sun);
                }
                else if (statu == 3)
                    rr.timeseg.emplace_back(ToTimeSeg(buff));
            }
            buff.clear();
        }
    }
    return r;
}

#endif