#ifndef DOSU_HIT_TYPE_H
#define DOSU_HIT_TYPE_H

#include "../../common.h"

// https://github.com/kionell/osu-classes/blob/08a608a2005d0e9671567d4b9ef6c1d452d2edcd/src/Objects/Enums/HitType.ts
enum class HitType {
    Normal = 0b1,
    Slider = 0b10,
    NewCombo = 0b100,
    Spinner = 0b1000,
};

constexpr auto operator+(HitType o) noexcept {
    return static_cast<std::underlying_type_t<HitType>>(o);
}

#endif