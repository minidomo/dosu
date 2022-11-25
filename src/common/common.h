#ifndef dosu_file_common
#define dosu_file_common

#include <Godot.hpp>

using namespace std;
using namespace godot;

#define dev_assert(condition)                                           \
    if (!(condition)) {                                                 \
        Godot::print_error("\"" #condition "\" is false", __FUNCTION__, \
                           __FILE__, __LINE__);                         \
    }

#define dev_register_method(class_name, function_name) \
    { register_method(#function_name, &class_name::function_name); }

// https://www.learncpp.com/cpp-tutorial/scoped-enumerations-enum-classes/#operatorplus
constexpr auto operator+(Error o) noexcept {
    return static_cast<std::underlying_type_t<Error>>(o);
}

#endif
