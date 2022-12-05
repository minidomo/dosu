#include "./timeline_circle.h"

void TimelineCircle::_register_methods() {
    dev_register_method(TimelineCircle, _ready);
}

void TimelineCircle::_init() {}

void TimelineCircle::_ready() { body = get_node<TextureRect>("Body"); }

void TimelineCircle::set_color(Color color) {
    color.a = get_modulate().a;
    this->color = color;
    body->set_modulate(color);
}

Color TimelineCircle::get_color() { return color; }

void TimelineCircle::set_opacity(float opacity) {
    auto modulate = get_modulate();
    modulate.a = opacity;
    body->set_modulate(modulate);
}

float TimelineCircle::get_opacity() { return body->get_modulate().a; }
