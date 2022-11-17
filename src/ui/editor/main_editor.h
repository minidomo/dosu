#ifndef DOSU_MAIN_EDITOR_H
#define DOSU_MAIN_EDITOR_H

#include <Button.hpp>
#include <Node.hpp>

#include "../../common.h"

class MainEditor : public Node {
    GODOT_CLASS(MainEditor, Node);

   private:
   public:
    static void _register_methods();
    void _init();

    void _ready();
};

#endif