@echo off
mkdir build>nul 2>nul
g++ -Wall -static  .\AutoTask.cpp -o .\build\AutoTask.exe -mwindows -std=c++20
strip .\build\AutoTask.exe
echo.finished!
pause>nul