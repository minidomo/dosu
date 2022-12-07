#ifndef dosu_file_main_editor
#define dosu_file_main_editor

#include <ColorRect.hpp>
#include <Control.hpp>
#include <InputEvent.hpp>
#include <Label.hpp>
#include <Ref.hpp>
#include <vector>

#include "common/common.h"
#include "object/conductor.h"
#include "ui/background.h"
#include "ui/editor/beat_snap_divisor.h"
#include "ui/editor/body/compose_body.h"
#include "ui/editor/body/song_setup_body.h"
#include "ui/editor/body/timing_body.h"
#include "ui/editor/object_timeline.h"
#include "ui/editor/play_area.h"
#include "ui/editor/tab.h"
#include "ui/editor/timeline.h"

class MainEditor : public Control {
    GODOT_CLASS(MainEditor, Control);

   private:
    Background *background;
    Conductor *conductor;

    vector<Tab *> tabs;
    vector<Control *> bodies;

    Label *time_label;
    Label *progress_label;

    Timeline *timeline;

    ComposeBody *compose_body;
    SongSetupBody *song_setup_body;
    TimingBody *timing_body;
    ObjectTimeline *object_timeline;
    PlayArea *play_area;

    BeatSnapDivisor *beat_snap_divisor;

    int tab_index;

    void init_tabs();
    void init_bodies();
    void init_icon_buttons();
    void init_conductor();
    void init_timeline_zoom_buttons();

   public:
    static void _register_methods();
    void _init();

    void _ready();
    void _process(float delta);

    void on_tab_clicked(int index);
    void on_icon_button_pressed(int index);
    void on_song_position_updated(float song_position);
    void on_timeline_click(float percent);
    void on_files_dropped(PoolStringArray files, int screen);
    void on_timeline_zoom_button_pressed(float value);
};

#endif