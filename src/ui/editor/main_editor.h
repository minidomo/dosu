#ifndef dosu_file_main_editor
#define dosu_file_main_editor

#include <Control.hpp>
#include <Label.hpp>
#include <vector>

#include "common/common.h"
#include "object/conductor.h"
#include "ui/background.h"
#include "ui/editor/tab.h"

class MainEditor : public Control {
    GODOT_CLASS(MainEditor, Control);

   private:
    Background *background;
    Conductor *conductor;

    vector<Tab *> tabs;
    vector<Control *> bodies;

    Label *time_label;
    Label *progress_label;

    int tab_index;

    void init_tabs();
    void init_bodies();
    void init_icon_buttons();
    void init_conductor();

   public:
    static void _register_methods();
    void _init();

    void _ready();

    void on_tab_clicked(int index);
    void on_icon_button_pressed(int index);
    void on_song_position_update(int64_t song_position);
};

#endif