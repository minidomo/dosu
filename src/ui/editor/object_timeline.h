#ifndef dosu_file_object_timeline
#define dosu_file_object_timeline

#include <Control.hpp>
#include <PackedScene.hpp>
#include <Ref.hpp>

#include "common/common.h"
#include "object/conductor.h"
#include "ui/editor/timeline_tick.h"

class ObjectTimeline : public Control {
    GODOT_CLASS(ObjectTimeline, Control);

   private:
    Conductor *conductor;
    Control *tick_container;

    Ref<PackedScene> tick_object;

   public:
    static void _register_methods();
    void _init();
    void _ready();

    void draw_ticks(float percent_missing, int64_t beat_number);
    void set_tick_position(TimelineTick *tick, float x);

    void set_conductor(Conductor *conductor);
    Conductor *get_conductor();

    void on_song_position_updated(float song_position);
};

#endif