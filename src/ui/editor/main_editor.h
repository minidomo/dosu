#ifndef dosu_file_main_editor
#define dosu_file_main_editor

#include <Button.hpp>
#include <Control.hpp>
#include <vector>

#include "common/common.h"
#include "object/conductor.h"
#include "ui/editor/tab.h"

class MainEditor : public Control {
    GODOT_CLASS(MainEditor, Control);

   private:
    Conductor *conductor;
    vector<Tab *> tabs;
    vector<Control *> bodies;

    int tab_index;

    void init_tabs();
    void init_bodies();

   public:
    static void _register_methods();
    void _init();

    void _ready();

    void on_tab_clicked(int index);
};

#endif