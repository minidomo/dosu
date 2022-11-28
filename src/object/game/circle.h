#ifndef dosu_file_circle
#define dosu_file_circle

#include <AudioStreamPlayer.hpp>
#include <Node2D.hpp>
#include <Sprite.hpp>

#include "common/common.h"

class Circle : public Node2D {
    GODOT_CLASS(Circle, Node2D);

   private:
    Sprite *body;
    Sprite *border;
    Sprite *approach_circle;
    Color color;
    AudioStreamPlayer *hit_sound;

   public:
    static void _register_methods();
    void _init();
    void _ready();
    void set_color(Color color);
    Color get_color();
};

#endif