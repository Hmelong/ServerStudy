#pragma once

#include <chrono>
#include "format.h"

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