#ifndef dosu_file_conductor
#define dosu_file_conductor

#include <AudioStreamPlayer.hpp>
#include <Timer.hpp>
#include <utility>

#include "common/common.h"
#include "object/enum/conductor_go_type.h"

/**
 * https://github.com/LegionGames/Conductor-Example/blob/master/Scripts/Conductor.gd
 */
class Conductor : public AudioStreamPlayer {
    GODOT_CLASS(Conductor, AudioStreamPlayer);

   private:
    float bpm;
    int64_t measures;

    float song_position;
    int64_t song_position_in_beats;
    float seconds_per_beat;
    int64_t last_reported_beat;
    int64_t beats_before_start = 0;
    int64_t measure;

    int64_t closest;
    float time_off_beat;

    Timer *start_timer;

    float calculate_song_position();

   public:
    static void _register_methods();
    void _init();

    void _ready();
    void _process(float delta);

    void report_beat();
    void play_with_beat_offset(int64_t beats);
    pair<int64_t, float> closest_beat(float nth);
    void play_from_beat(int64_t beat, int64_t offset);
    void on_timeout();

    void set_bpm(float bpm);
    void set_measures(int64_t measures);

    float get_bpm();
    int64_t get_measures();

    float get_seconds_per_beat();
    float get_total_duration();
    float get_song_position();
    Dictionary get_beat(float position, float song_offset, int64_t beat_offset,
                        int64_t beat_divisor);

    void toggle_pause();
    void go_to(float time, ConductorGoType action);
    void go_to_percent(float percent, ConductorGoType action);
    bool is_actually_playing();
};

#endif