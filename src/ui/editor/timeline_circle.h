#ifndef dosu_file_timeline_circle
#define dosu_file_timeline_circle

#include <Control.hpp>
#include <TextureRect.hpp>

#include "common/common.h"

class TimelineCircle : public Control {
    GODOT_CLASS(TimelineCircle, Control);

   private:
    TextureRect *body;

    Color color;

   public:
    static void _register_methods();
    void _init();
    void _ready();
    void set_color(Color color);
    Color get_color();
    void set_opacity(float opacity);
    float get_opacity();
};

#endif