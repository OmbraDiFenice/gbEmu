#pragma once

#include <ctime>
#include <iostream>

#define OUT std::cout
#define ERR std::cerr

#define TRACE 0
#define DBG   1
#define INFO  2
#define WARN  3
#define ERROR 4

#define LOG(stream, level, msg)                                                \
    if (level >= LOG_LEVEL) {                                                  \
        std::time_t current_time = std::time(nullptr);                         \
        char buf[20];                                                          \
        std::strftime(buf, sizeof(buf), "%F %T",                               \
                      std::localtime(&current_time));                          \
        stream << buf << " [" << #level << "] " << __FILE__ << ":" << __LINE__ \
               << " - " << msg << "\n";                                        \
    }

#define LOG_TRACE(msg) LOG(OUT, TRACE, msg)
#define LOG_DBG(msg)   LOG(OUT, DBG, msg)
#define LOG_INFO(msg)  LOG(OUT, INFO, msg)
#define LOG_WARN(msg)  LOG(ERR, WARN, msg)
#define LOG_ERROR(msg) LOG(ERR, ERROR, msg)

#define TRACE_CALL LOG_TRACE(__FUNCTION__)