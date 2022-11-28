#include "./circle.h"

void Circle::_register_methods() { dev_register_method(Circle, _ready); }

void Circle::_init() {}

void Circle::_ready() {
    body = get_node<Sprite>("Body");
    border = get_node<Sprite>("Border");
    approach_circle = get_node<Sprite>("ApproachCircle");
    hit_sound = get_node<AudioStreamPlayer>("HitSound");

    set_color(Color::hex(0x7eff8cff));
}

void Circle::set_color(Color color) {
    this->color = color;
    body->set_modulate(color);
    approach_circle->set_modulate(color);
}

Color Circle::get_color() { return color; }

void Circle::set_opacity(float opacity) {
    this->opacity = opacity;
    set_modulate(Color(255, 255, 255, opacity));
}

float Circle::get_opacity() { return opacity; }

void Circle::play_hit_sound() { hit_sound->play(); }

void Circle::set_hit_object(HitObject *hit_object) {
    this->hit_object = hit_object;
}

HitObject *Circle::get_hit_object() { return hit_object; }
