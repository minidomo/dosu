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