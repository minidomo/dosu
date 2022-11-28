#ifndef dosu_file_play_area
#define dosu_file_play_area

#include <Control.hpp>

#include "common/common.h"

class PlayArea : public Control {
    GODOT_CLASS(PlayArea, Control);

   private:
   public:
    static void _register_methods();
    void _init();

    void _ready();
};

#endif