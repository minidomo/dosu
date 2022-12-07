#ifndef dosu_file_main_menu
#define dosu_file_main_menu

#include <Button.hpp>
#include <Node.hpp>

#include "common/common.h"

class MainMenu : public Node {
    GODOT_CLASS(MainMenu, Node);

   private:
    Button *play_button;
    Button *edit_button;
    Button *exit_button;
    Button *open_game_folder_button;

   public:
    static void _register_methods();
    void _init();

    void _ready();

    void on_play_button_pressed();
    void on_edit_button_pressed();
    void on_exit_button_pressed();
    void on_open_game_folder_button_pressed();
};

#endif