#pragma once
#include <utils/Logging.h>
#include <stdexcept>

#define ASSERT(cond, msg)                      \
    if (!(cond)) {                             \
        LOG_ERROR(#cond << ": " << msg); \
        throw std::logic_error(msg);           \
    }
