#include "./timeline_tick.h"

void TimelineTick::_register_methods() {
    dev_register_method(TimelineTick, _ready);
}

void TimelineTick::_init() {
    alpha = .5f;
    color = Color(255, 255, 255, alpha);
    height = 6;
}

void TimelineTick::_ready() {
    background = get_node<ColorRect>("ColorRect");
    set_color(color);
    set_height(height);
}

Color TimelineTick::get_color() { return color; }

void TimelineTick::set_color(Color color) {
    color.a = alpha;
    this->color = color;
    background->set_frame_color(color);
}

int TimelineTick::get_height() { return height; }

void TimelineTick::set_height(int height) {
    this->height = height;
    set_size(Vector2(1, (float)height));
}
