#ifndef PIO_HPP
#define PIO_HPP

#include <iostream>
#include <windows.h>
#include "Ptime.hpp"

constexpr int P_BLACK = 0;
constexpr int P_BLUE = 1;
constexpr int P_GREEN = 2;
constexpr int P_LGREEN = 3;
constexpr int P_RED = 4;
constexpr int P_PURPLE = 5;
constexpr int P_YELLOW = 6;
constexpr int P_WHITE = 7;
constexpr int P_GREY = 8;
constexpr int P_BBLUE = 9;
constexpr int P_BGREEN = 10;
constexpr int P_BLGREEN = 11;
constexpr int P_BRED = 12;
constexpr int P_BPURPLR = 13;
constexpr int P_BYELLOW = 14;
constexpr int P_EWHITE = 15;

void Pprint(const auto &output, int x = 7)
{
    if (x >= 0 && x <= 15)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
    else
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), P_WHITE);
    auto tm = GetCurTM();
    std::cout <<  output;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), P_WHITE);
    return;
}

void PDEBUGprint(const auto &output)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), P_GREEN);
    auto tm = GetCurTM();
    std::cout << tm->tm_hour << ':' << tm->tm_min << ':' << tm->tm_sec << '\t' << output;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), P_WHITE);
    return;
}

#endif