#pragma once

#define BIND_FN(fn) std::bind(&fn, this, std::placeholders::_1)
