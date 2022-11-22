#ifndef dosu_file_tab
#define dosu_file_tab

#include <Button.hpp>
#include <ColorRect.hpp>
#include <Control.hpp>

#include "common/common.h"

class Tab : public Control {
    GODOT_CLASS(Tab, Control);

   private:
    String title;

    ColorRect *background;
    Button *button;

   public:
    static void _register_methods();
    void _init();

    void _ready();

    Button *get_button();
    void select();
    void deselect();
};

#endif