#include "./tab.h"

void Tab::_register_methods() {
    register_method("_ready", &Tab::_ready);
    register_property<Tab, String>("title", &Tab::title, "");
}

void Tab::_init() { title = ""; }

void Tab::_ready() {
    background = get_node<ColorRect>("ColorRect");
    button = get_node<Button>("Button");

    button->set_text(title);
}

Button *Tab::get_button() { return button; }

void Tab::select() { background->set_frame_color(Color::hex(0x2081f8ff)); }

void Tab::deselect() { background->set_frame_color(Color::hex(0x202020ff)); }