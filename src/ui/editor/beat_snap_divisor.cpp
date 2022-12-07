#include "./beat_snap_divisor.h"

#include "singleton/map_manager.h"
#include "singleton/scene_manager.h"

void BeatSnapDivisor::_register_methods() {
    dev_register_method(BeatSnapDivisor, _ready);
    dev_register_method(BeatSnapDivisor, on_value_changed);
}

void BeatSnapDivisor::_init() { value = 4; }

void BeatSnapDivisor::_ready() {
    value_label = get_node<Label>("Value");
    slider = get_node<HSlider>("HSlider");

    SceneManager::get_singleton(this)->recursive_scale_font(this);

    slider->connect("value_changed", this, "on_value_changed");
}

void BeatSnapDivisor::set_value(int64_t value) {
    slider->set_value((float)value);
}

int64_t BeatSnapDivisor::get_value() { return value; }

void BeatSnapDivisor::on_value_changed(float value) {
    this->value = round_value(value);
    value_label->set_text("1/" + String::num_int64(this->value));

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_beat_divisor(this->value);
}

int64_t BeatSnapDivisor::round_value(float value) {
    int64_t val = (int64_t)Math::clamp<float>(value, 1, 16);

    switch (val) {
        case 10:
        case 11:
            val = 12;
            break;
        case 13:
        case 14:
        case 15:
            val = 16;
            break;
        default:
            break;
    }

    return val;
}

void BeatSnapDivisor::initialize() {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    set_value(beatmap->get_beat_divisor());
}