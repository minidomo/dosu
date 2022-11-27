#ifndef dosu_file_object_timeline
#define dosu_file_object_timeline

#include <Control.hpp>
#include <PackedScene.hpp>
#include <Ref.hpp>
#include <tuple>
#include <vector>

#include "common/common.h"
#include "object/conductor.h"
#include "object/timing_point.h"
#include "ui/editor/timeline_tick.h"

class ObjectTimeline : public Control {
    GODOT_CLASS(ObjectTimeline, Control);

   private:
    Dictionary tick_color_schemes;

    Conductor *conductor;
    Control *tick_container;

    Ref<PackedScene> tick_object;

    vector<tuple<float, int64_t>> determine_ticks(float offset,
                                                  float snap_length,
                                                  int64_t beat_number);
    void setup_tick(TimelineTick *tick, tuple<float, int64_t> tick_data,
                    int64_t meter, int64_t beat_divisor);
    void draw_ticks(float percent_missing, int64_t beat_number,
                    TimingPoint *control_point);

    Color get_tick_color(int64_t beat_divisor, int64_t index);
    void init_tick_color_schemes();

   public:
    static void _register_methods();
    void _init();
    void _ready();

    void set_conductor(Conductor *conductor);
    Conductor *get_conductor();

    void on_song_position_updated(float song_position);
};

#endif