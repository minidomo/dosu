#ifndef dosu_file_conductor
#define dosu_file_conductor

#include <AudioStreamPlayer.hpp>
#include <Timer.hpp>
#include <utility>

#include "common/common.h"

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
    float sec_per_beat;
    int64_t last_reported_beat;
    int64_t beats_before_start = 0;
    int64_t measure;

    int64_t closest;
    float time_off_beat;

    Timer *start_timer;

   public:
    static void _register_methods();
    void _init();

    void _ready();
    void _physics_process(real_t delta);

    void report_beat();
    void play_with_beat_offset(int64_t beats);
    pair<int64_t, float> closest_beat(float nth);
    void play_from_beat(int64_t beat, int64_t offset);
    void on_timeout();

    void set_bpm(float bpm);
    void set_measures(int64_t measures);

    float get_bpm();
    int64_t get_measures();
};

#endif