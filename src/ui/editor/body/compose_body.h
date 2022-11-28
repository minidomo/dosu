#ifndef dosu_file_compose_body
#define dosu_file_compose_body

#include <Control.hpp>

#include "common/common.h"

class ComposeBody : public Control {
    GODOT_CLASS(ComposeBody, Control);

   private:
   public:
    static void _register_methods();
    void _init();

    void _ready();
};

#endif