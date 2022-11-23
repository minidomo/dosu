#ifndef dosu_file_conductor_go_type
#define dosu_file_conductor_go_type

#include "common/common.h"

enum class ConductorGoType {
    Play,
    Pause,
    Maintain,
};

constexpr auto operator+(ConductorGoType o) noexcept {
    return static_cast<std::underlying_type_t<ConductorGoType>>(o);
}

#endif