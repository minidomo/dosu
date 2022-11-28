#ifndef dosu_file_circle
#define dosu_file_circle

#include <AudioStreamPlayer.hpp>
#include <Node2D.hpp>
#include <Sprite.hpp>

#include "common/common.h"
#include "object/hit_object.h"

class Circle : public Node2D {
    GODOT_CLASS(Circle, Node2D);

   private:
    Sprite *body;
    Sprite *border;
    Sprite *approach_circle;
    AudioStreamPlayer *hit_sound;

    HitObject *hit_object;
    Color color;
    float opacity;

   public:
    static void _register_methods();
    void _init();
    void _ready();
    void set_color(Color color);
    Color get_color();
    void set_opacity(float opacity);
    float get_opacity();
    void play_hit_sound();
    void set_hit_object(HitObject *hit_object);
    HitObject *get_hit_object();
};

#endif