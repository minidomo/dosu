#ifndef dosu_file_main_editor
#define dosu_file_main_editor

#include <Button.hpp>
#include <Node.hpp>

#include "common/common.h"

class MainEditor : public Node {
    GODOT_CLASS(MainEditor, Node);

   private:
   public:
    static void _register_methods();
    void _init();

    void _ready();
};

#endif