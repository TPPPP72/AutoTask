#include "config.hpp"

int main()
{
    Config config;
    config.Read("new.json");
    Stage set;
    while (1)
    {
        Stage stage(Ptime::GetCurTM());
        if (!set.curdt.is_empty())
            stage.curdt = set.curdt;
        if (!set.curtp.is_empty())
            stage.curtp = set.curtp;
        if (!set.curwd.is_empty())
            stage.curwd = set.curwd;
        config.Launch(stage);
    }
    return 0;
}