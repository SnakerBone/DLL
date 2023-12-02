//
// Created by snake on 1/12/2023.
//
#include "logger.h"

#include <iostream>

void print(const std::string &str)
{
    std::cout << "[SNKR/DLL]: " << str;
}

void println(const std::string &str)
{
    std::cout << "[SNKR/DLL]: " << str << '\n';
}

void printerr(const std::string &str)
{
    std::cerr << "[SNKR/DLL]: " << str << '\n';
}
