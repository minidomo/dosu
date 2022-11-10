#ifndef DOSU_MAIN_MENU_H
#define DOSU_MAIN_MENU_H

#include <Node.hpp>

#include "../common.h"

class MainMenu : public Node {
    GODOT_CLASS(MainMenu, Node);

   private:
   public:
    static void _register_methods();
    void _init();
};

#endif