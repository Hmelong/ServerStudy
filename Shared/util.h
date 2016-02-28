#pragma once

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <chrono>
#include "format.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define BUFSIZE 1024
#define MAX_BUF_SIZE 65535

struct msg
{
    int no;
    string message;

    short size()
    {
        short size = sizeof(int) + message.length();
        return size;
    }
};


enum PACKET_NO
{
    NO_0 = 0,
    NO_1,

    MAX_NO
};

#define LOG_INFO(...)  LOG("[INFO ]", __FUNCTION__, __VA_ARGS__)
#define LOG_WARN(...)  LOG("[WARN ]", __FUNCTION__, __VA_ARGS__)
#define LOG_ERROR(...) LOG("[ERROR]", __FUNCTION__, __VA_ARGS__)

template<typename... ARGS>
void LOG(const std::string& logHeader,
    const std::string& funcName,
    const ARGS&... args)
{
    std::tm local_tm;
    auto timeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    localtime_s(&local_tm, &timeT);
    std::cout << fmt::format("{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d} ",
        local_tm.tm_year + 1900,
        local_tm.tm_mon + 1,
        local_tm.tm_mday,
        local_tm.tm_hour,
        local_tm.tm_min,
        local_tm.tm_sec);

    std::cout
        << logHeader 
        << "[" << funcName << "] "
        << fmt::format(args...) << std::endl;
}
