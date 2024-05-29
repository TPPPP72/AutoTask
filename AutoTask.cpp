#include <thread>
#include "includes\winruntime.hpp"
#include "includes\Ptime.hpp"
#include "includes\DataGroup.hpp"
#include "includes\Pconf.hpp"
#include "includes\PIO.hpp"

const bool DEBUG = true;
Rule GetCurRule(const std::vector<Rule> &rules, int wday);

int main()
{
    std::vector<Rule> rules = GetRules();
    int wday = GetCurTM()->tm_wday;
    Rule Crule = GetCurRule(rules, wday);
    std::thread Update(
        [&]()
        {
            while (1)
            {
                int wd = GetCurTM()->tm_wday;
                if (wd != wday)
                {
                    if (DEBUG)
                        Pprint("Day switched\n", P_BLUE);
                    wday = wd;
                    Crule = GetCurRule(rules, wday);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(60000));
            }
        });
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    int now = GetCurTimeVal();
    while (1)
    {
        for (auto &i : Crule.timeseg)
        {
            while (now >= i.st && now <= i.ed)
            {
                for (auto &j : Crule.command)
                    Run(j);
                if (DEBUG)
                    Pprint("Run command. TimeVal:" + std::to_string(now) + '\n', P_BLUE);
                std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                now = GetCurTimeVal();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(60000));
        now = GetCurTimeVal();
    }
    return 0;
}

Rule GetCurRule(const std::vector<Rule> &rules, int wday)
{
    for (auto i = 0; i < rules.size(); i++)
        for (auto &j : rules[i].date)
            if (j == wday)
            {
                if (DEBUG)
                    Pprint("rule:" + std::to_string(i) + '\n' + "wday:" + std::to_string(wday) + '\n', P_BLUE);
                return rules[i];
            }
    return rules[0];
}