#pragma once

#include <stdexcept>

#define ASSERT(cond, msg)                      \
    if (!(cond)) {                             \
        LOG_ERROR("" << #cond << ": " << msg); \
        throw std::logic_error(msg);           \
    }
