#ifndef DOSU_MAIN_EDIT_H
#define DOSU_MAIN_EDIT_H

#include <Button.hpp>
#include <Node.hpp>

#include "../common.h"

class MainEdit : public Node {
    GODOT_CLASS(MainEdit, Node);

   private:
    Button *select_button;
    NodePath select_button_path;

    Button *back_button;
    NodePath back_button_path;

   public:
    static void _register_methods();
    void _init();

    void _ready();

    void on_select_button_pressed();
    void on_back_button_pressed();
    void on_files_dropped(PoolStringArray files, int screen);
};

#endif