#include "./slidable_row.h"

#include "common/util.h"

void SlidableRow::_register_methods() {
    register_method("_ready", &SlidableRow::_ready);
    register_method("on_value_changed", &SlidableRow::on_value_changed);

    register_property<SlidableRow, String>("title", &SlidableRow::title, "");
    register_property<SlidableRow, String>("description",
                                           &SlidableRow::description, "");
}

void SlidableRow::_init() {
    title = "";
    description = "";
}

void SlidableRow::_ready() {
    title_label = get_node<Label>("Title");
    description_label = get_node<Label>("Description");
    value_label = get_node<Label>("Value");
    slider = get_node<HSlider>("HSlider");

    slider->connect("value_changed", this, "on_value_changed");

    title_label->set_text(title);
    description_label->set_text(description);
}

void SlidableRow::on_value_changed(float value) {
    value_label->set_text(Util::limit_decimal(value, 1));
}

HSlider *SlidableRow::get_slider() { return slider; }

float SlidableRow::get_value() { return slider->get_value(); }

void SlidableRow::set_value(float value) { slider->set_value(value); }