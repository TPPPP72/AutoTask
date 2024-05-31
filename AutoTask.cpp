#include <thread>
#include <algorithm>
#include <mutex>
#include "includes\winruntime.hpp"
#include "includes\Ptime.hpp"
#include "includes\DataGroup.hpp"
#include "includes\Pconf.hpp"
#include "includes\PIO.hpp"

const bool DEBUG = false;
Schedule GetSchedule(const std::vector<Rule> &rules, int wday);
std::timed_mutex mtx;

int main()
{
    std::vector<Rule> rules = GetRules();
    int wday = GetCurTM()->tm_wday, now;
    bool sync = false;
    Schedule today = GetSchedule(rules, wday);
    std::thread Update(
        [&]()
        {
            while (1)
            {
                now = GetCurTimeVal();
                if (DEBUG)
                    PDEBUGprint("Time Updated!\n");
                if (!sync)
                {
                    if (DEBUG)
                        PDEBUGprint("Syncing......\n");
                    int ss = GetCurTM()->tm_sec;
                    if (DEBUG)
                        PDEBUGprint("Sleeping......\n");
                    std::this_thread::sleep_for(std::chrono::seconds(60 - ss));
                    if (DEBUG)
                        PDEBUGprint("Sync Finished!\n");
                    now = GetCurTimeVal();
                    if (DEBUG)
                        PDEBUGprint("Time Updated!\n");
                    sync = true;
                }
                int wd = GetCurTM()->tm_wday;
                if (wd != wday)
                {
                    wday = wd;
                    std::unique_lock<std::timed_mutex> lk(mtx, std::defer_lock);
                    if (lk.try_lock_for(std::chrono::seconds(60)))
                    {
                        today = GetSchedule(rules, wday);
                        if (DEBUG)
                            PDEBUGprint("Day switched!\n");
                        sync = false;
                    }
                }
                std::this_thread::sleep_for(std::chrono::seconds(60));
            }
        });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    while (1)
    {
        bool isfindtask = false;
        {
            std::lock_guard<std::timed_mutex> lg(mtx);
            if (today.tasklist.size() == 0){
                if(DEBUG)
                    PDEBUGprint("No Rule\n");
                std::this_thread::sleep_for(std::chrono::seconds(30));
                continue;
            }
            for (auto &i : today.tasklist)
            {
                if (now >= i.timeseg.st && now <= i.timeseg.ed)
                {
                    for (auto &j : i.command)
                    {
                        if (DEBUG)
                            PDEBUGprint("Run command:" + j + '\n');
                        else
                            Run(j);
                    }
                    isfindtask = true;
                    std::this_thread::sleep_for(std::chrono::milliseconds(i.sleep));
                }
            }
        }
        if (!isfindtask)
        {
            if (DEBUG)
                PDEBUGprint("No task\n");
            std::this_thread::sleep_for(std::chrono::seconds(30));
        }
    }
    return 0;
}

Schedule GetSchedule(const std::vector<Rule> &rules, int wday)
{
    std::vector<Rule> select;
    Schedule res;
    for (auto i = 0; i < rules.size(); i++)
        for (auto &j : rules[i].date)
            if (j == wday)
            {
                if (DEBUG)
                    PDEBUGprint("rule:" + std::to_string(i) + '\n');
                select.emplace_back(rules[i]);
            }
    if (select.size() == 0)
        return res;
    for (auto &i : select)
        for (auto &j : i.timeseg)
            res.tasklist.emplace_back(Task{j, i.command, i.sleep});
    std::sort(res.tasklist.begin(), res.tasklist.end(), [](Task A, Task B)
              { return A.timeseg.st < B.timeseg.st; });
    return res;
}