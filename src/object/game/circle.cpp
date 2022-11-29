#include "./circle.h"

#include "singleton/map_manager.h"

void Circle::_register_methods() { dev_register_method(Circle, _ready); }

void Circle::_init() { opacity = 1; }

void Circle::_ready() {
    body = get_node<Sprite>("Body");
    border = get_node<Sprite>("Border");
    approach_circle = get_node<Sprite>("ApproachCircle");
    hit_sound = get_node<AudioStreamPlayer>("HitSound");

    base_size = (int64_t)body->get_rect().get_size().x;

    color = Color::hex(0x7eff8cff);
    set_color(color);
    set_opacity(opacity);
}

void Circle::set_color(Color color) {
    color.a = 1;
    this->color = color;
    body->set_modulate(color);
    approach_circle->set_modulate(color);
}

Color Circle::get_color() { return color; }

void Circle::set_opacity(float opacity) {
    this->opacity = opacity;
    auto prev = get_modulate();
    prev.a = opacity;
    set_modulate(prev);
}

float Circle::get_opacity() { return opacity; }

void Circle::play_hit_sound() { hit_sound->play(); }

void Circle::set_hit_object(HitObject *hit_object) {
    this->hit_object = hit_object;
}

HitObject *Circle::get_hit_object() { return hit_object; }

void Circle::set_circle_size(float circle_size) {
    int64_t px =
        MapManager::get_singleton(this)->circle_size_to_pixel(circle_size);
    float scale = (float)px / base_size;
    set_scale(Vector2(scale, scale));
}

void Circle::set_approach_circle_visible(bool visible) {
    approach_circle->set_visible(visible);
}

void Circle::approach_circle_standard_position() {
    approach_circle->set_scale(Vector2(1.2f, 1.2f));
}

void Circle::set_approach_circle_progress(float percent) {
    float max_scale = 4;
    float min_scale = 1.05;
    float diff = max_scale - min_scale;
    float scale = min_scale + diff * (1 - percent);
    approach_circle->set_scale(Vector2(scale, scale));
}