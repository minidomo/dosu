#include "./timing_body.h"

#include <ResourceLoader.hpp>

#include "common/util.h"
#include "object/enum/conductor_go_type.h"
#include "object/timing_point.h"
#include "singleton/map_manager.h"

void TimingBody::_register_methods() {
    dev_register_method(TimingBody, _ready);
    dev_register_method(TimingBody, on_add_timing_point_button_pressed);
    dev_register_method(TimingBody, on_delete_timing_point_button_pressed);
    dev_register_method(TimingBody, on_timing_point_row_pressed);
    dev_register_method(TimingBody, on_timing_points_updated);
    dev_register_method(TimingBody, on_current_time_button_pressed);
    dev_register_method(TimingBody, on_time_input_entry_text_entered);
    dev_register_method(TimingBody, on_bpm_input_entry_text_entered);
    dev_register_method(TimingBody, on_time_signature_input_entry_text_entered);
}

void TimingBody::_init() { select_time = 0; }

void TimingBody::_ready() {
    add_timing_point_button =
        get_node<BaseButton>("LeftBar/TimingPointButtons/AddButton");
    delete_timing_point_button =
        get_node<BaseButton>("LeftBar/TimingPointButtons/DeleteButton");
    timing_point_row_container =
        get_node<Control>("LeftBar/TimingPoints/ScrollContainer/VBoxContainer");
    current_time_button =
        get_node<BaseButton>("Body/TimeContainer/CurrentTimeButton");
    time_input_entry =
        get_node<TimingBodyInputEntry>("Body/TimeContainer/Time");
    bpm_input_entry = get_node<TimingBodyInputEntry>("Body/Bpm");
    time_signature_input_entry =
        get_node<TimingBodyInputEntry>("Body/TimeSignature");

    Util::recursive_scale_font(get_node<Control>("Body"));

    timing_point_row_object = ResourceLoader::get_singleton()->load(
        "res://scenes/editor/TimingPointRow.tscn");

    add_timing_point_button->connect("pressed", this,
                                     "on_add_timing_point_button_pressed");
    delete_timing_point_button->connect(
        "pressed", this, "on_delete_timing_point_button_pressed");

    current_time_button->connect("pressed", this,
                                 "on_current_time_button_pressed");
    time_input_entry->get_input()->connect("text_entered", this,
                                           "on_time_input_entry_text_entered");
    bpm_input_entry->get_input()->connect("text_entered", this,
                                          "on_bpm_input_entry_text_entered");
    time_signature_input_entry->get_input()->connect(
        "text_entered", this, "on_time_signature_input_entry_text_entered");

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->connect("timing_points_updated", this, "on_timing_points_updated");
}

void TimingBody::initialize() {
    draw_timing_point_rows();
    select_row(0, false);
}

void TimingBody::set_conductor(Conductor *conductor) {
    this->conductor = conductor;
}

Conductor *TimingBody::get_conductor() { return conductor; }

void TimingBody::on_add_timing_point_button_pressed() {
    int64_t time = Util::to_milliseconds(conductor->get_song_position());
    select_time = time;

    auto timing_point = TimingPoint::_new();
    timing_point->init_red_line();
    timing_point->set_time(time);

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->add_timing_point(timing_point);
}

void TimingBody::on_delete_timing_point_button_pressed() {
    int index = find_selected_row_index();
    if (index == -1) return;

    // always have at least one timing point, so dont delete last one
    if (timing_point_row_container->get_child_count() == 1) return;

    auto row = Object::cast_to<TimingPointRow>(
        timing_point_row_container->get_child(index));
    select_time = row->get_time();

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->remove_timing_point(row->get_time());
}

void TimingBody::draw_timing_point_rows() {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    auto timing_points = beatmap->get_timing_points();

    int64_t diff =
        timing_points.size() - timing_point_row_container->get_child_count();

    if (diff < 0) {
        // delete
        for (int i = 0; i < -diff; i++) {
            Node *child = timing_point_row_container->get_child(0);
            timing_point_row_container->remove_child(child);
            child->queue_free();
        }
    } else if (diff > 0) {
        // add
        for (int i = 0; i < diff; i++) {
            auto row = Object::cast_to<TimingPointRow>(
                timing_point_row_object->instance());
            timing_point_row_container->add_child(row);
        }
    }

    Array children = timing_point_row_container->get_children();
    for (int i = 0; i < children.size(); i++) {
        auto row = Object::cast_to<TimingPointRow>(children[i]);
        setup_timing_point_row(row, timing_points[i], i);
    }
}

void TimingBody::setup_timing_point_row(TimingPointRow *row, TimingPoint *data,
                                        int index) {
    row->set_time(data->get_time());
    row->set_bpm(data->get_bpm());
    row->set_meter(data->get_meter());
    row->color_scheme_deselect();

    auto button = row->get_button();

    if (button->is_connected("pressed", this, "on_timing_point_row_pressed")) {
        button->disconnect("pressed", this, "on_timing_point_row_pressed");
    }
    button->connect("pressed", this, "on_timing_point_row_pressed",
                    Array::make(index));
}

void TimingBody::on_timing_point_row_pressed(int index) {
    auto row = Object::cast_to<TimingPointRow>(
        timing_point_row_container->get_child(index));
    select_time = row->get_time();

    select_row(index, true);
}

void TimingBody::select_row(int index, bool seek) {
    deselect_all_rows();

    auto row = Object::cast_to<TimingPointRow>(
        timing_point_row_container->get_child(index));
    row->color_scheme_select();

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    auto timing_points = beatmap->get_timing_points();
    auto timing_point = timing_points[index];

    time_input_entry->set_value(String::num_int64(timing_point->get_time()));
    bpm_input_entry->set_value(
        String::num_real(timing_point->get_bpm()).pad_decimals(2));
    time_signature_input_entry->set_value(
        String::num_int64(timing_point->get_meter()));

    if (seek) {
        float time = Math::clamp<float>(Util::to_seconds(row->get_time()), 0,
                                        conductor->get_total_duration());
        conductor->go_to(time, ConductorGoType::Maintain);
    }
}

void TimingBody::deselect_all_rows() {
    Array children = timing_point_row_container->get_children();
    for (int i = 0; i < children.size(); i++) {
        auto row = Object::cast_to<TimingPointRow>(children[i]);
        row->color_scheme_deselect();
    }
}

void TimingBody::on_timing_points_updated() {
    draw_timing_point_rows();
    select_row(find_index_for_time(select_time), false);
}

int TimingBody::find_selected_row_index() {
    Array children = timing_point_row_container->get_children();
    for (int i = 0; i < children.size(); i++) {
        auto row = Object::cast_to<TimingPointRow>(children[i]);
        if (row->is_selected()) {
            return i;
        }
    }

    return -1;
}

int TimingBody::find_index_for_time(int64_t time) {
    int ret = 0;

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    auto timing_points = beatmap->get_timing_points();

    for (int i = 0; i < timing_points.size(); i++) {
        if (timing_points[i]->get_time() <= time) {
            ret = i;
        }
    }

    return ret;
}

void TimingBody::on_current_time_button_pressed() {
    int64_t time = Util::to_milliseconds(conductor->get_song_position());
    update_time(time);
}

void TimingBody::update_time(int64_t time) {
    int index = find_selected_row_index();
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    auto timing_points = beatmap->get_timing_points();
    auto old_timing_point = timing_points[index];

    auto new_timing_point = TimingPoint::_new();
    new_timing_point->copy(old_timing_point);
    new_timing_point->set_time(time);

    beatmap->remove_timing_point(old_timing_point->get_time());
    select_time = time;
    beatmap->add_timing_point(new_timing_point);
}

void TimingBody::on_time_input_entry_text_entered(String value) {
    int64_t time = value.to_int();
    update_time(time);
}

void TimingBody::on_bpm_input_entry_text_entered(String value) {
    float bpm = String::num_real(value.to_float()).pad_decimals(2).to_float();

    if (bpm < 1) {
        bpm_input_entry->set_value(bpm_input_entry->get_value());
    } else {
        int index = find_selected_row_index();
        auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
        auto timing_points = beatmap->get_timing_points();
        auto timing_point = timing_points[index];

        timing_point->set_bpm(bpm);
        beatmap->emit_signal("timing_points_updated");
    }
}

void TimingBody::on_time_signature_input_entry_text_entered(String value) {
    int64_t meter = value.to_int();

    if (meter < 1) {
        time_signature_input_entry->set_value(
            time_signature_input_entry->get_value());
    } else {
        int index = find_selected_row_index();
        auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
        auto timing_points = beatmap->get_timing_points();
        auto timing_point = timing_points[index];

        timing_point->set_meter(meter);
        beatmap->emit_signal("timing_points_updated");
    }
}
