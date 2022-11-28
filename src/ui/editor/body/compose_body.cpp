#include "./compose_body.h"

void ComposeBody::_register_methods() {
    dev_register_method(ComposeBody, _ready);
    dev_register_method(ComposeBody, on_circle_button_pressed);
}

void ComposeBody::_init() {}

void ComposeBody::_ready() {
    circle_button = get_node<BaseButton>("LeftBar/CircleButton");

    circle_button->connect("pressed", this, "on_circle_button_pressed");
}

void ComposeBody::on_circle_button_pressed() {
    Godot::print("pressed circle button");
    // add circle to beatmap at current song position
    // update timeline
    // update play area
}