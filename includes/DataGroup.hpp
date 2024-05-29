#ifndef DATA_GROUP_HPP
#define DATA_GROUP_HPP

#include <string>
#include <vector>
#include "Ptime.hpp"

struct Rule
{
    std::vector<int> date;
    std::vector<TimeSeg> timeseg;
    std::vector<std::string> command;
    void clear(){
        date.clear();
        timeseg.clear();
        command.clear();
    }
};

#endif
