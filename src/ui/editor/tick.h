#ifndef dosu_file_timeline_tick
#define dosu_file_timeline_tick

#include <ColorRect.hpp>
#include <Control.hpp>

#include "common/common.h"

class Tick : public Control {
    GODOT_CLASS(Tick, Control);

   private:
    ColorRect *background;

    Color color;
    int height;

    float opacity;

   public:
    static void _register_methods();
    void _init();

    void _ready();

    Color get_color();
    void set_color(Color color);

    int get_height();
    void set_height(int height);

    void set_opacity(float opacity);
    float get_opacity();
};

#endif
