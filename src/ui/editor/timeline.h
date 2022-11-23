#ifndef dosu_file_timeline
#define dosu_file_timeline

#include <ColorRect.hpp>
#include <Control.hpp>
#include <InputEvent.hpp>
#include <Ref.hpp>

#include "common/common.h"

class Timeline : public Control {
    GODOT_CLASS(Timeline, Control);

   private:
    ColorRect *playhead;

   public:
    static void _register_methods();
    void _init();

    void _ready();
    void _gui_input(Ref<InputEvent> event);

    void set_playhead_progress(float percent);
    void set_playhead_position(float x);
};

#endif