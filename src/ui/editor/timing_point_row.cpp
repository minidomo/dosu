#include "./timing_point_row.h"

#include <DynamicFont.hpp>

#include "common/util.h"

void TimingPointRow::_register_methods() {
    dev_register_method(TimingPointRow, _ready);
}

void TimingPointRow::_init() {}

void TimingPointRow::_ready() {
    button = get_node<BaseButton>("Button");
    background = get_node<Control>("Background");
    time_label = get_node<Label>("Time");
    metadata_label = get_node<Label>("Preview/Metadata");

    Vector2 size = get_size();
    size.height = Util::scale_value_by_resolution(32, 1080);
    set_custom_minimum_size(size);

    auto font_time_label =
        Object::cast_to<DynamicFont>(time_label->get("custom_fonts/font"));
    font_time_label->set_size(
        (int64_t)Util::scale_value_by_resolution(16, 1080));

    auto font_metadata_label =
        Object::cast_to<DynamicFont>(metadata_label->get("custom_fonts/font"));
    font_metadata_label->set_size(
        (int64_t)Util::scale_value_by_resolution(12, 1080));
}

void TimingPointRow::color_scheme_select() {
    selected = true;
    background->set_modulate(Color::hex(0x33cc99ff));
}

void TimingPointRow::color_scheme_deselect() {
    selected = false;
    background->set_modulate(Color::hex(0xffffff00));
}

void TimingPointRow::set_bpm(float bpm) {
    this->bpm = bpm;
    update_metadata();
}

void TimingPointRow::set_meter(int64_t meter) {
    this->meter = meter;
    update_metadata();
}

void TimingPointRow::set_time(int64_t time) {
    this->time = time;
    time_label->set_text(Util::to_timestamp(time));
}

float TimingPointRow::get_bpm() { return bpm; }

int64_t TimingPointRow::get_meter() { return meter; }

int64_t TimingPointRow::get_time() { return time; }

void TimingPointRow::update_metadata() {
    String text_bpm = String::num_real(bpm).pad_decimals(1);
    String text_meter = String::num_int64(meter);
    metadata_label->set_text(text_bpm + " bpm " + text_meter + "/4");
}

BaseButton *TimingPointRow::get_button() { return button; }

bool TimingPointRow::is_selected() { return selected; }