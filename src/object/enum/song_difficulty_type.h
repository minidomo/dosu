#ifndef dosu_file_song_difficulty_type
#define dosu_file_song_difficulty_type

#include "common/common.h"

enum class SongDifficultyType {
    HpDrainRate,
    CircleSize,
    ApproachRate,
    OverallDifficulty,
};

constexpr auto operator+(SongDifficultyType o) noexcept {
    return static_cast<std::underlying_type_t<SongDifficultyType>>(o);
}

#endif