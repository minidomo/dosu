#ifndef dosu_file_icon_type
#define dosu_file_icon_type

#include "common/common.h"

enum class IconType {
    SeekStartPlay,
    TogglePause,
    SeekStartPause,
};

constexpr auto operator+(IconType o) noexcept {
    return static_cast<std::underlying_type_t<IconType>>(o);
}

#endif