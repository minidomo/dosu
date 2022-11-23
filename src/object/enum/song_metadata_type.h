#ifndef dosu_file_song_metadata_type
#define dosu_file_song_metadata_type

#include "common/common.h"

enum class SongMetadataType {
    Artist,
    ArtistUnicode,
    Title,
    TitleUnicode,
    Creator,
    Difficulty,
    Source,
    Tags,
};

constexpr auto operator+(SongMetadataType o) noexcept {
    return static_cast<std::underlying_type_t<SongMetadataType>>(o);
}

#endif