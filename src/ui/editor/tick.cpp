#include "./tick.h"

void Tick::_register_methods() { dev_register_method(Tick, _ready); }

void Tick::_init() {
    opacity = .5f;
    color = Color(255, 255, 255, opacity);
    height = 8;
}

void Tick::_ready() {
    background = get_node<ColorRect>("ColorRect");
    set_color(color);
    set_height(height);
}

Color Tick::get_color() { return color; }

void Tick::set_color(Color color) {
    color.a = opacity;
    this->color = color;
    background->set_frame_color(color);
}

int Tick::get_height() { return height; }

void Tick::set_height(int height) {
    this->height = height;
    set_size(Vector2(1, (float)height));
}

void Tick::set_opacity(float opacity) {
    this->opacity = opacity;
    set_color(color);
}

float Tick::get_opacity() { return opacity; }
