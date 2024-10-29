#ifndef WINRUNTIME_HPP
#define WINRUNTIME_HPP

#include <string>
#include <windows.h>

inline int Run(const std::string &command, bool is_block = true) {
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));
  if (!CreateProcess(NULL, (LPSTR)&command[0], NULL, NULL, FALSE,
                     CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
    return -1;
  DWORD ExitCode = 1;
  if (is_block) {
    WaitForSingleObject(pi.hProcess, INFINITE);
    GetExitCodeProcess(pi.hProcess, &ExitCode);
  }
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  return ExitCode;
}

#endif