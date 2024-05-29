#ifndef PSTR_HPP
#define PSTR_HPP

#include <string>

std::string SpaceClear(std::string str){
    while (str.front() == ' ')
        str.erase(str.begin());
    while (str.back() == ' ')
        str.erase(str.end() - 1);
    return str;
}

#endif