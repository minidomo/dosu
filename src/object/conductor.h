#ifndef dosu_file_conductor
#define dosu_file_conductor

#include <AudioStreamPlayer.hpp>
#include <Timer.hpp>
#include <utility>

#include "common/common.h"
#include "object/enum/conductor_go_type.h"

/**
 * https://docs.godotengine.org/en/3.3/tutorials/audio/sync_with_audio.html
 */
class Conductor : public AudioStreamPlayer {
    GODOT_CLASS(Conductor, AudioStreamPlayer);

   private:
    float song_position;

    int64_t closest;
    float time_off_beat;

    float calculate_song_position();

   public:
    static void _register_methods();
    void _init();

    void _ready();
    void _process(float delta);

    float get_total_duration();
    float get_song_position();
    Dictionary get_beat(float position, float song_offset, int64_t beat_offset,
                        int64_t beat_divisor, float bpm);

    void toggle_pause();
    void go_to(float time, ConductorGoType action);
    void go_to_percent(float percent, ConductorGoType action);
    bool is_actually_playing();
};

#endif