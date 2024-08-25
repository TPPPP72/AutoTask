#pragma once

#include <string>
#include <vector>
#include <thread>
#include "Ptime.hpp"
#include "json.hpp"
#include "winruntime.hpp"

struct Stage
{
    Stage() : curdt(Ptime::Date::GetEmpty()), curtp(Ptime::TimePoint::GetEmpty()), curwd(Ptime::Wday::GetEmpty()) {}
    Stage(tm *tm) : curdt(tm), curtp(tm), curwd(tm) {}
    Ptime::Date curdt;
    Ptime::TimePoint curtp;
    Ptime::Wday curwd;
    static Stage GetCurrent()
    {
        return Ptime::GetCurTM();
    }
};

struct Command
{
public:
    std::vector<std::string> content;
    bool is_block;
};

struct Task
{
    uint32_t id;
    std::vector<Ptime::Date> Date;
    std::vector<Ptime::TimeSeg> Time;
    std::vector<Ptime::Wday> Wday;
    Command Command;
    bool is_loop = false;
    void Launch(const Stage &stage)
    {
        if (!is_loop && is_trigged)
            return;
        auto Trig = [&]()
        {
            if (!Date.empty())
            {
                bool check = false;
                for (auto &i : Date)
                {
                    if (stage.curdt == i)
                    {
                        check = true;
                        break;
                    }
                }
                if (!check)
                    return false;
            }
            // Wday Comparison
            if (!Wday.empty())
            {
                bool check = false;
                for (auto &i : Wday)
                {
                    if (stage.curwd == i)
                    {
                        check = true;
                        break;
                    }
                }
                if (!check)
                    return false;
            }
            // TimeSeg Comparison
            if (!Time.empty())
            {
                bool check = false;
                for (auto &i : Time)
                {
                    if (i.contain(stage.curtp))
                    {
                        check = true;
                        break;
                    }
                }
                if (!check)
                    return false;
            }
            return true;
        };
        if (Trig())
        {
            is_trigged = true;
            for (auto &i : Command.content)
            {
                if (Command.is_block)
                {
                    Run(i);
                }
                else
                {
                    std::thread thread(Run, i);
                    thread.detach();
                }
            }
        }
    }

private:
    bool is_trigged = false;
};

namespace nlohmann
{
    template <>
    struct adl_serializer<Task>
    {
        static Task from_json(const ordered_json &j)
        {
            Task t;
            if (j.contains("Date"))
                std::for_each(j["Date"].items().begin(), j["Date"].items().end(), [&](const auto &i)
                              { t.Date.emplace_back(static_cast<std::string>(i.value())); });
            if (j.contains("Time"))
                std::for_each(j["Time"].items().begin(), j["Time"].items().end(), [&](const auto &i)
                              { t.Time.emplace_back(static_cast<std::string>(i.value())); });
            if (j.contains("Wday"))
                std::for_each(j["Wday"].items().begin(), j["Wday"].items().end(), [&](const auto &i)
                              { t.Wday.emplace_back(static_cast<std::string>(i.value())); });
            std::for_each(j["Command"]["content"].items().begin(), j["Command"]["content"].items().end(), [&](const auto &i)
                          { t.Command.content.emplace_back(static_cast<std::string>(i.value())); });
            t.Command.is_block = j["Command"]["is_block"];
            t.is_loop = j["is_loop"];
            return t;
        }
    };
}

void to_json(nlohmann::ordered_json &j, const Task &t)
{
    std::for_each(t.Date.begin(), t.Date.end(), [&](const auto &i)
                  { j["Date"].emplace_back(i.ToStr()); });
    std::for_each(t.Time.begin(), t.Time.end(), [&](const auto &i)
                  { j["Time"].emplace_back(i.ToStr()); });
    std::for_each(t.Wday.begin(), t.Wday.end(), [&](const auto &i)
                  { j["Wday"].emplace_back(i.ToStr()); });
    std::for_each(t.Command.content.begin(), t.Command.content.end(), [&](const auto &i)
                  { j["Command"]["content"].emplace_back(i); });
    j["Command"]["is_block"] = t.Command.is_block;
    j["is_loop"] = t.is_loop;
}