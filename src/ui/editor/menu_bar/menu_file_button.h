#ifndef dosu_file_menu_file_button
#define dosu_file_menu_file_button

#include <MenuButton.hpp>

#include "common/common.h"

class MenuFileButton : public MenuButton {
    GODOT_CLASS(MenuFileButton, MenuButton);

   private:
   public:
    static void _register_methods();
    void _init();

    void _ready();

    void on_index_pressed(int64_t index);
};

#endif