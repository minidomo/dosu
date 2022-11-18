#ifndef DOSU_MENU_FILE_BUTTON
#define DOSU_MENU_FILE_BUTTON

#include <MenuButton.hpp>

#include "../../common.h"

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