#include "./timeline.h"

#include <InputEventMouseButton.hpp>

void Timeline::_register_methods() {
    register_method("_ready", &Timeline::_ready);
    register_method("_gui_input", &Timeline::_gui_input);

    register_signal<Timeline>("timeline_click", "percent",
                              GODOT_VARIANT_TYPE_REAL);
}

void Timeline::_init() {}

void Timeline::_ready() { playhead = get_node<ColorRect>("Playhead"); }

void Timeline::_gui_input(Ref<InputEvent> event) {
    if (event->is_action_pressed("primary_click")) {
        auto ev = Object::cast_to<InputEventMouseButton>(event.ptr());

        float percent = ev->get_position().x / get_rect().size.width;
        emit_signal("timeline_click", percent);
    }
}

void Timeline::set_playhead_progress(float percent) {
    float pos_x = get_rect().size.width * percent - 1;
    set_playhead_position(pos_x);
}

void Timeline::set_playhead_position(float x) {
    Vector2 pos = playhead->get_position();
    playhead->set_position(Vector2(x, pos.y));
}