#ifndef dosu_file_compose_body
#define dosu_file_compose_body

#include <BaseButton.hpp>
#include <Control.hpp>

#include "common/common.h"

class ComposeBody : public Control {
    GODOT_CLASS(ComposeBody, Control);

   private:
    BaseButton *circle_button;

   public:
    static void _register_methods();
    void _init();

    void _ready();
    void on_circle_button_pressed();
};

#endif