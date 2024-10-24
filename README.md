# AutoTask
This is a software that allows you to customize your PC's startup tasks and time-trigged tasks.  

## Using
https://github.com/nlohmann/json

## How to config
The configuration file for this program is encoded in json and has the following keys:  
1.interval (sets the interval at which the program runs, Int)  
2.Startup (sets the program's startup task, Task)  
3.Rule (sets the program's runtime task, Task)  
  
Where, for Task has the following contents:  
1.Date (the date condition of the trigger, if you don't set it, the default is unconditional, “2024.8.23”, string list)  
2.Time (the time period condition of the trigger, if not set then the default is unconditional, “00:00-12:00”,string list)  
3.Wday (trigger week day condition, default is unconditional if not set, “Mon”,string list)  
4. Command (the command to be executed, call Cmd, Command)  
5. is_loop (set whether the task runs in a loop, bool)  
  
Among them, for Command has the following content:  
1.content (the specific content of the command, string list)  
2.is_block (select blocking or non-blocking process to run, bool)  

Here's a example:
```json
//rule.json
{
    "interval": 1000,
    "Startup":{
        "Task1":{
            "Wday":["Sun"],
            "Command":{
                "content":[
                    "start 1.exe"
                ]
            },
            "is_loop": false
        }
    },
    "Rule": {
        "Task1": {
            "Time": [
                "0:0-15:0",
                "16:0-16:8",
                "20:10-20:23",
                "21:55-23:59"
            ],
            "Wday": [
                "Sun"
            ],
            "Command": {
                "content": [
                    "start 2.exe"
                ],
                "is_block": true
            },
            "is_loop": false
        }
    }
}
```

## How to build
With cmake,you can just use `cmake --build` to build this project.  
But you should make sure that your compiler supports for the option `-std=c++17`.

---
**Pull requests welcome.**
