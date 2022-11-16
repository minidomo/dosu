#ifndef DOSU_MAIN_MENU_H
#define DOSU_MAIN_MENU_H

#include <Button.hpp>
#include <Node.hpp>

#include "../common.h"

class MainMenu : public Node {
    GODOT_CLASS(MainMenu, Node);

   private:
    Button *play_button;
    NodePath play_button_path;

    Button *edit_button;
    NodePath edit_button_path;

    Button *exit_button;
    NodePath exit_button_path;

   public:
    static void _register_methods();
    void _init();

    void _ready();

    void on_play_button_pressed();
    void on_edit_button_pressed();
    void on_exit_button_pressed();
};

#endif