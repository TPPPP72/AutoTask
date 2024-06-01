#ifndef DATA_GROUP_HPP
#define DATA_GROUP_HPP

#include <string>
#include <vector>
#include "Ptime.hpp"

struct Startup
{
    std::vector<std::string> command;
    int sleep = -1;
};

struct Rule
{
    std::vector<int> date;
    std::vector<TimeSeg> timeseg;
    std::vector<std::string> command;
    int sleep = -1;
    void clear()
    {
        date.clear();
        timeseg.clear();
        command.clear();
        sleep = -1;
    }
};

struct Plan
{
    int time;
    std::vector<std::string> command;
    int sleep = -1;
};

struct Task
{
    TimeSeg timeseg;
    std::vector<std::string> command;
    int sleep;
};

struct Schedule
{
    std::vector<Task> tasklist;
    void clear()
    {
        tasklist.clear();
    }
};

#endif
