#ifndef dosu_file_hit_sound
#define dosu_file_hit_sound

#include "common/common.h"

// https://github.com/kionell/osu-classes/blob/master/src/Objects/Enums/HitSound.ts
enum class HitSound {
    None = 0b0,
    Normal = 0b1,
    Whistle = 0b10,
    Finish = 0b100,
    Clap = 0b1000,
};

constexpr auto operator+(HitSound o) noexcept {
    return static_cast<std::underlying_type_t<HitSound>>(o);
}

#endif