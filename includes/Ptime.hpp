#ifndef P_TIME_HPP
#define P_TIME_HPP

#include <iostream>
#include <chrono>

enum Day
{
    Sun,
    Mon,
    Tue,
    Wed,
    Thu,
    Fri,
    Sat
};

struct TimeSeg
{
    int st;
    int ed;
};

int ToTimeVal(const std::string &str)
{
    int h, m;
    std::sscanf(str.c_str(), "%d:%d", &h, &m);
    return h * 60 + m;
}

TimeSeg ToTimeSeg(const std::string &str)
{
    std::string str1 = str.substr(0, str.find('-')), str2 = str.substr(str.find('-') + 1);
    return TimeSeg{ToTimeVal(str1), ToTimeVal(str2)};
}

tm* GetCurTM(){
    auto tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return std::localtime(&tm);
}

int GetCurTimeVal()
{
    auto tm = GetCurTM();
    return tm->tm_hour*60+tm->tm_min;
}

#endif