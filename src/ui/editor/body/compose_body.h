#ifndef dosu_file_compose_body
#define dosu_file_compose_body

#include <BaseButton.hpp>
#include <Control.hpp>

#include "common/common.h"
#include "ui/editor/play_area.h"

class ComposeBody : public Control {
    GODOT_CLASS(ComposeBody, Control);

   private:
    BaseButton *circle_button;
    PlayArea *play_area;

   public:
    static void _register_methods();
    void _init();

    void _ready();
    void on_circle_button_pressed();
    PlayArea *get_play_area();
};

#endif