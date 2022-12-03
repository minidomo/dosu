#ifndef dosu_file_timeline
#define dosu_file_timeline

#include <ColorRect.hpp>
#include <Control.hpp>
#include <InputEvent.hpp>
#include <PackedScene.hpp>
#include <Ref.hpp>
#include <vector>

#include "common/common.h"
#include "object/conductor.h"
#include "ui/editor/tick.h"

class Timeline : public Control {
    GODOT_CLASS(Timeline, Control);

   private:
    ColorRect *playhead;
    Control *timing_points;
    Conductor *conductor;

    Ref<PackedScene> tick_object;

    vector<Dictionary> determine_timing_point_data();
    void setup_timing_point(Tick *timing_point, Dictionary timing_point_data);

   public:
    static void _register_methods();
    void _init();

    void _ready();
    void _gui_input(Ref<InputEvent> event);

    void set_playhead_progress(float percent);
    void set_playhead_position(float x);
    void on_song_position_updated(float song_position);
    void on_timing_points_updated();
    void set_conductor(Conductor *conductor);
    Conductor *get_conductor();
    void initialize();
};

#endif