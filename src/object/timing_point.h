#ifndef dosu_file_timing_point
#define dosu_file_timing_point

#include <Node.hpp>

#include "common/common.h"

class TimingPoint : public Node {
    GODOT_CLASS(TimingPoint, Node);

   private:
    int64_t time;
    float beat_length;
    int64_t meter;
    bool uninherited;

    /* green lines */
    float bpm_multiplier;
    float slider_velocity;

    /* red lines */
    float bpm;

    static float calculate_bpm(float beat_length);
    static float calculate_beat_length(float bpm);

   public:
    static void _register_methods();
    void _init();

    void parse_line(String line);
    String to_file_string();

    void init_red_line();

    void set_time(int64_t time);
    void set_beat_length(float beat_length);
    void set_meter(int64_t meter);
    void set_uninherited(bool uninherited);
    void set_bpm(float bpm);

    void copy(TimingPoint *timing_point);
};

#endif