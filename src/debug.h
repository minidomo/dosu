#ifndef DOSU_DEBUG_H
#define DOSU_DEBUG_H

#include <Godot.hpp>

#define dev_assert(condition)                                                  \
    if (!(condition)) {                                                        \
        godot::Godot::print_error("\"" #condition "\" is false", __FUNCTION__, \
                                  __FILE__, __LINE__);                         \
    }

#endif