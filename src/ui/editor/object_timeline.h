#ifndef dosu_file_object_timeline
#define dosu_file_object_timeline

#include <Control.hpp>
#include <PackedScene.hpp>
#include <Ref.hpp>
#include <vector>

#include "common/common.h"
#include "object/conductor.h"
#include "object/timing_point.h"
#include "ui/editor/tick.h"

class ObjectTimeline : public Control {
    GODOT_CLASS(ObjectTimeline, Control);

   private:
    Dictionary tick_color_schemes;

    Conductor *conductor;
    Control *tick_container;

    Ref<PackedScene> tick_object;

    bool hovering;

    float prev_percent_missing;
    int64_t prev_beat_number;
    TimingPoint *prev_control_point;

    vector<Dictionary> determine_ticks(float offset, float snap_length,
                                       int64_t beat_number);
    void setup_tick(Tick *tick, Dictionary tick_data, int64_t meter,
                    int64_t beat_divisor);
    void draw_ticks(float percent_missing, int64_t beat_number,
                    TimingPoint *control_point);

    Color get_tick_color(int64_t beat_divisor, int64_t index);
    void init_tick_color_schemes();

    vector<Dictionary> determine_timing_point_data();
    Dictionary determine_visibile_range(float song_position, float beat_length,
                                        int64_t beat_divisor,
                                        float timeline_zoom);

    float determine_x_position(int64_t time, Dictionary range);
    vector<Dictionary> determine_tick_data(Beatmap *beatmap,
                                           TimingPoint *control_point,
                                           Dictionary visible_range);

   public:
    static void _register_methods();
    void _init();
    void _ready();

    void set_conductor(Conductor *conductor);
    Conductor *get_conductor();

    void set_hovering(bool hovering);
    bool is_hovering();

    void on_song_position_updated(float song_position);
    void on_timeline_zoom_updated(float timeline_zoom);
    void on_timing_points_updated();
    void on_mouse_entered();
    void on_mouse_exited();
};

#endif