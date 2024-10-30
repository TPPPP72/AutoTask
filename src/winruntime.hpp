#ifndef WINRUNTIME_HPP
#define WINRUNTIME_HPP

#include <string>
#include <vector>
#include <windows.h>
#include <winnt.h>

inline int Start(const std::string &path, bool is_block) {
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));
  if (CreateProcess(static_cast<LPCSTR>(&path[0]), NULL, NULL, NULL, FALSE,
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

inline int StartWithParam(const std::string &command, bool is_block) {
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));
  std::vector<char> commandBuffer(command.begin(), command.end());
  commandBuffer.push_back('\0');
  if (!CreateProcess(NULL, static_cast<LPSTR>(commandBuffer.data()), NULL, NULL, FALSE,
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