#ifndef dosu_file_main_edit
#define dosu_file_main_edit

#include <Button.hpp>
#include <Node.hpp>

#include "common/common.h"

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