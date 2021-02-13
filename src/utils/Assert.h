#pragma once
#include <stdexcept>
#include <utils/Logging.h>

#define ASSERT(cond, msg)                \
    if (!(cond)) {                       \
        LOG_ERROR(#cond << ": " << msg); \
        throw std::logic_error(msg);     \
    }
