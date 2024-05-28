#include <thread>
#include "includes\winruntime.hpp"
#include "includes\Ptime.hpp"
#include "includes\DataGroup.hpp"
#include "includes\Pconf.hpp"

int main()
{
    std::vector<Rule> rules = GetRules();
    int wday = GetCurTM()->tm_wday, cur = -1;
    for (size_t i = 0; i < rules.size(); i++)
    {
        if (cur != -1)
            break;
        for (auto &j : rules[i].date)
            if (j == wday)
            {
                cur = i;
                break;
            }
    }
    Rule Crule = rules[cur];
    while (1)
    {
        int now = GetCurTimeVal();
        for (auto &i : Crule.timeseg)
        {
            while (now >= i.st && now <= i.ed)
            {
                for (auto &j : Crule.command)
                    Run(j);
                std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                now = GetCurTimeVal();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(60000));
    }
    return 0;
}

