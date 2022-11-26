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

void ObjectTimeline::draw_ticks(float percent_missing, int64_t beat_number) {
    Util::delete_children(tick_container);

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    auto timing_point = beatmap->get_timing_points()[0];

    float center_x = get_size().x / 2;
    float snap_length =
        timing_point->get_beat_length() / beatmap->get_beat_divisor();
    float offset = snap_length * percent_missing;

    float start = center_x + offset;
    float prev = start - snap_length;
    int64_t index;

    index = (beat_number - 1 + beatmap->get_beat_divisor()) %
            beatmap->get_beat_divisor();
    for (float pos_x = prev; pos_x >= 0; pos_x -= snap_length) {
        auto tick = Object::cast_to<TimelineTick>(tick_object->instance());
        tick_container->add_child(tick);

        if (index == 0) {
            tick->set_height(30);
        }

        set_tick_position(tick, pos_x);
        index = (index - 1 + beatmap->get_beat_divisor()) %
                beatmap->get_beat_divisor();
    }

    index = beat_number % beatmap->get_beat_divisor();
    for (float pos_x = start; pos_x < get_size().x; pos_x += snap_length) {
        auto tick = Object::cast_to<TimelineTick>(tick_object->instance());
        tick_container->add_child(tick);

        if (index == 0) {
            tick->set_height(30);
        }

        set_tick_position(tick, pos_x);
        index = (index + 1) % beatmap->get_beat_divisor();
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
    auto timing_point = beatmap->get_timing_points()[0];

    float offset = Util::to_seconds(timing_point->get_time());
    Dictionary beat_info = conductor->get_beat(song_position, offset, 1, 1);
    float next_time = beat_info["time"];
    int64_t beat_number = beat_info["index"];

    float diff = next_time - song_position;
    float percent_missing = diff / conductor->get_seconds_per_beat();

    draw_ticks(percent_missing, beat_number);
}