#pragma once

#include "task.h"
#include "json.hpp"
#include <fstream>

struct Config
{

    void Read(const std::string &str)
    {
        std::ifstream rd(str);
        nlohmann::ordered_json json;
        rd >> json;
        rd.close();

        auto PeekforTask = [](const nlohmann::ordered_json &json, std::vector<Task> &task_list)
        {
            std::for_each(json.items().begin(), json.items().end(), [&](const auto &i)
                          { Task t = i.value().template get<Task>();t.id=static_cast<uint32_t>(i.key().back()-'0') ;task_list.emplace_back(t); });
        };
        if (json.contains("interval"))
            interval = json["interval"];
        if (json.contains("Startup"))
            PeekforTask(json["Startup"], Startup);
        if (json.contains("Rule"))
            PeekforTask(json["Rule"], Rule);
    }

    void Write(const std::string &str) const
    {
        auto TaskAssign = [](nlohmann::ordered_json &json, const std::vector<Task> &task_list)
        {
            std::for_each(task_list.begin(), task_list.end(), [&](const auto &i)
                          { nlohmann::ordered_json j;to_json(j, i);json.emplace_back(j); });
        };
        nlohmann::ordered_json json;
        json["interval"] = interval;
        if (!Startup.empty())
            TaskAssign(json["Startup"], Startup);
        if (!Rule.empty())
            TaskAssign(json["Rule"], Rule);
    }

    void Launch(const Stage &stage)
    {
        if (!had_startup_run)
        {
            std::for_each(Startup.begin(), Startup.end(), [&](auto &i)
                          { i.Launch(stage); });
            had_startup_run = true;
        }
        std::for_each(Rule.begin(), Rule.end(), [&](auto &i)
                      { i.Launch(stage); });
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
    }

    unsigned int interval = 1000;
    std::vector<Task> Startup;
    std::vector<Task> Rule;
    bool had_startup_run = false;
};