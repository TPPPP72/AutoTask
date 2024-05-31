# AutoTask
This is a software that allows you to customize your PC's startup plans and do some constant works.  

## How to config
Currently, it support the constant time-slot tasks.  
Your config file should be named as `rule.conf`.  
All your config should be included in `[]`.  
Each rule must have $4$ parameters:`Command`, `Date`, `Time`, `Sleep`.  
When you declare a parameter in the first level [], that parameter will be given to **all rules**.
The following is the syntax for declaring each parameter:
```
//Command (call cmd.exe to run)
Command{commmand1,command2}

//Date (Sun,Mon,Tue,Wed,Thu,Fri,Sat)
Date{Mon,Fri,Sat}

//Time (xx:xx-xx:xx)
Time{7:00-12:00,14:00-18:00}

//Sleep (ms)
Sleep{3000}
```
Here is a simply full file sample:  
```
//rule.conf
[Command{taskkill /f /im 1.exe},[Date{Mon},Time{7:00-9:00}],[Date{Tue,Wed,Thu},Time{9:00-11:00}]]
```
It means executing the command “taskkill /f /im 1.exe” on Monday from 7:00 am to 9:00 am and on Tuesday, Wednesday and Thursday from 9:00 am to 11:00 am.

## How to build
This is a C++ project, please config a compiler in your environment variables.  
And you can build this project using `build.bat`.

---
**Pull requests welcome.**
