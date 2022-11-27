#include "./object_timeline.h"

#include <ResourceLoader.hpp>

#include "common/util.h"
#include "singleton/map_manager.h"

void ObjectTimeline::_register_methods() {
    dev_register_method(ObjectTimeline, _ready);
    dev_register_method(ObjectTimeline, set_tick_position);
    dev_register_method(ObjectTimeline, on_song_position_updated);
}

void ObjectTimeline::_init() {}

void ObjectTimeline::_ready() {
    tick_container = get_node<Control>("Ticks");

    tick_object = ResourceLoader::get_singleton()->load(
        "res://scenes/editor/TimelineTick.tscn");
}

void ObjectTimeline::draw_ticks(float percent_missing, int64_t beat_number,
                                TimingPoint *control_point) {
    // Util::delete_children(tick_container);

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();

    // float center_x = get_size().x / 2;
    float snap_length = control_point->get_beat_length() /
                        control_point->get_meter() /
                        beatmap->get_beat_divisor();
    float offset = snap_length * percent_missing;

    // float start = center_x + offset;
    // float prev = start - snap_length;
    // int64_t index;

    // int64_t ticks_per_measure =
    //     control_point->get_meter() * beatmap->get_beat_divisor();

    // index = (beat_number - 1 + ticks_per_measure) % ticks_per_measure;
    // for (float pos_x = prev; pos_x >= 0; pos_x -= snap_length) {
    //     auto tick = Object::cast_to<TimelineTick>(tick_object->instance());
    //     tick_container->add_child(tick);

    //     if (index == 0) {
    //         tick->set_height(30);
    //     }

    //     set_tick_position(tick, pos_x);
    //     index = (index - 1 + ticks_per_measure) % ticks_per_measure;
    // }

    // index = beat_number % ticks_per_measure;
    // for (float pos_x = start; pos_x < get_size().x; pos_x += snap_length) {
    //     auto tick = Object::cast_to<TimelineTick>(tick_object->instance());
    //     tick_container->add_child(tick);

    //     if (index == 0) {
    //         tick->set_height(30);
    //     }

    //     set_tick_position(tick, pos_x);
    //     index = (index + 1) % ticks_per_measure;
    // }

    auto tick_data = determine_ticks(offset, snap_length, beat_number);
    int64_t diff = tick_data.size() - tick_container->get_child_count();

    if (diff < 0) {
        // delete ticks
        for (int i = 0; i < -diff; i++) {
            Node *child = tick_container->get_child(0);
            tick_container->remove_child(child);
            child->queue_free();
        }
    } else if (diff > 0) {
        // add ticks
        for (int i = 0; i < diff; i++) {
            auto tick = Object::cast_to<TimelineTick>(tick_object->instance());
            tick_container->add_child(tick);
        }
    }

    Array children = tick_container->get_children();
    for (int i = 0; i < children.size(); i++) {
        auto tick = Object::cast_to<TimelineTick>(children[i]);
        setup_tick(tick, tick_data[i], control_point->get_meter(),
                   beatmap->get_beat_divisor());
    }
}

void ObjectTimeline::set_tick_position(TimelineTick *tick, float x) {
    Vector2 pos;
    pos.x = x;
    pos.y = get_size().y - tick->get_height();
    tick->set_position(pos);
}

void ObjectTimeline::set_conductor(Conductor *conductor) {
    if (this->conductor != nullptr) {
        this->conductor->disconnect("song_position_updated", this,
                                    "on_song_position_updated");
    }

    this->conductor = conductor;

    if (this->conductor != nullptr) {
        this->conductor->connect("song_position_updated", this,
                                 "on_song_position_updated");
    }
}

Conductor *ObjectTimeline::get_conductor() { return conductor; }

void ObjectTimeline::on_song_position_updated(float song_position) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    auto control_point = beatmap->get_control_point_for_time(
        Util::to_milliseconds(song_position));

    float offset = Util::to_seconds(control_point->get_time());
    Dictionary beat_info = conductor->get_beat(song_position, offset, 1,
                                               beatmap->get_beat_divisor());

    float next_time = beat_info["time"];
    int64_t beat_number = beat_info["index"];
    float step = beat_info["step"];

    float diff = next_time - song_position;
    float percent_missing = diff / step;

    draw_ticks(percent_missing, beat_number, control_point);
}

vector<tuple<float, int64_t>> ObjectTimeline::determine_ticks(
    float offset, float snap_length, int64_t beat_number) {
    vector<tuple<float, int64_t>> ret;

    float center_x = get_size().x / 2;
    float start = center_x + offset;
    float prev = start - snap_length;
    int64_t index;

    index = beat_number - 1;
    for (float pos_x = prev; pos_x >= 0; pos_x -= snap_length) {
        ret.push_back(make_tuple(pos_x, index));
        index--;
    }

    index = beat_number;
    for (float pos_x = start; pos_x < get_size().x; pos_x += snap_length) {
        ret.push_back(make_tuple(pos_x, index));
        index++;
    }

    return ret;
}

void ObjectTimeline::setup_tick(TimelineTick *tick,
                                tuple<float, int64_t> tick_data, int64_t meter,
                                int64_t beat_divisor) {
    int64_t ticks_per_measure = meter * beat_divisor;

    int64_t index = std::get<1>(tick_data);
    if (index % ticks_per_measure == 0) {
        tick->set_height(30);
        tick->set_color(Color::hex(0xffffffff));
    } else if (index % beat_divisor == 0) {
        tick->set_height(14);
        tick->set_color(Color::hex(0xffffffff));
    } else {
        tick->set_height(6);
        tick->set_color(Color::hex(0xEF1017ff));
    }

    Vector2 pos;
    pos.x = std::get<0>(tick_data);
    pos.y = get_size().y - tick->get_height();
    tick->set_position(pos);
}