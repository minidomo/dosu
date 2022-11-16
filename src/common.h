#ifndef DOSU_COMMON_H
#define DOSU_COMMON_H

#include <Godot.hpp>

using namespace std;
using namespace godot;

#define dev_assert(condition)                                           \
    if (!(condition)) {                                                 \
        Godot::print_error("\"" #condition "\" is false", __FUNCTION__, \
                           __FILE__, __LINE__);                         \
    }

#endif
