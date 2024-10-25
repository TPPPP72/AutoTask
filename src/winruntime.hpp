#ifndef WINRUNTIME_HPP
#define WINRUNTIME_HPP

#include <windows.h>
#include <string>

inline int Run(const std::string &command)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess(NULL, (LPSTR)&command[0], NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
        return -1;
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD ExitCode;
    GetExitCodeProcess(pi.hProcess, &ExitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return ExitCode;
}

#endif