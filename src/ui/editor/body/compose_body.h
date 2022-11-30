#ifndef dosu_file_compose_body
#define dosu_file_compose_body

#include <BaseButton.hpp>
#include <Control.hpp>

#include "common/common.h"
#include "ui/editor/play_area.h"

class ComposeBody : public Control {
    GODOT_CLASS(ComposeBody, Control);

   private:
    BaseButton *select_button;
    BaseButton *circle_button;
    BaseButton *delete_button;

    BaseButton *none_button;
    BaseButton *whistle_button;
    BaseButton *finish_button;
    BaseButton *clap_button;

    PlayArea *play_area;

   public:
    static void _register_methods();
    void _init();

    void _ready();
    PlayArea *get_play_area();

    void on_circle_button_pressed();
    void on_select_button_pressed();
    void on_delete_button_pressed();

    void on_none_button_pressed();
    void on_whistle_button_pressed();
    void on_finish_button_pressed();
    void on_clap_button_pressed();
};

#endif