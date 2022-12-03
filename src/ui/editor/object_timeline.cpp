#include "./object_timeline.h"

#include <ResourceLoader.hpp>

#include "common/util.h"
#include "singleton/map_manager.h"

void ObjectTimeline::_register_methods() {
    dev_register_method(ObjectTimeline, _ready);
    dev_register_method(ObjectTimeline, on_song_position_updated);
    dev_register_method(ObjectTimeline, on_timeline_zoom_updated);
    dev_register_method(ObjectTimeline, on_timing_points_updated);
    dev_register_method(ObjectTimeline, on_mouse_entered);
    dev_register_method(ObjectTimeline, on_mouse_exited);
}

void ObjectTimeline::_init() { init_tick_color_schemes(); }

void ObjectTimeline::_ready() {
    tick_container = get_node<Control>("Ticks");

    tick_object =
        ResourceLoader::get_singleton()->load("res://scenes/editor/Tick.tscn");

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->connect("timeline_zoom_updated", this, "on_timeline_zoom_updated");
    beatmap->connect("timing_points_updated", this, "on_timing_points_updated");

    connect("mouse_entered", this, "on_mouse_entered");
    connect("mouse_exited", this, "on_mouse_exited");
}

void ObjectTimeline::draw_ticks(float percent_missing, int64_t beat_number,
                                TimingPoint *control_point) {
    prev_percent_missing = percent_missing;
    prev_beat_number = beat_number;
    prev_control_point = control_point;

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();

    float snap_length =
        control_point->get_beat_length() / control_point->get_meter() /
        beatmap->get_beat_divisor() * beatmap->get_timeline_zoom();
    float offset = snap_length * percent_missing;

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
            auto tick = Object::cast_to<Tick>(tick_object->instance());
            tick_container->add_child(tick);
        }
    }

    Array children = tick_container->get_children();
    for (int i = 0; i < children.size(); i++) {
        auto tick = Object::cast_to<Tick>(children[i]);
        setup_tick(tick, tick_data[i], control_point->get_meter(),
                   beatmap->get_beat_divisor());
    }
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

/**
 * @return a vector of Dictionaries containing data on each tick.
 * - x: float - the x position of the tick
 * - index: int64_t - the index of the beat this tick is on (can be negative)
 */
vector<Dictionary> ObjectTimeline::determine_ticks(float offset,
                                                   float snap_length,
                                                   int64_t beat_number) {
    vector<Dictionary> ret;

    float center_x = get_size().x / 2;
    float start = center_x + offset;
    float prev = start - snap_length;
    int64_t index;

    index = beat_number - 1;
    for (float pos_x = prev; pos_x >= 0; pos_x -= snap_length) {
        ret.push_back(Dictionary::make("x", pos_x, "index", index));
        index--;
    }

    index = beat_number;
    for (float pos_x = start; pos_x < get_size().x; pos_x += snap_length) {
        ret.push_back(Dictionary::make("x", pos_x, "index", index));
        index++;
    }

    return ret;
}

void ObjectTimeline::setup_tick(Tick *tick, Dictionary tick_data, int64_t meter,
                                int64_t beat_divisor) {
    int64_t ticks_per_measure = meter * beat_divisor;

    int64_t index = tick_data["index"];
    if (index % ticks_per_measure == 0) {
        tick->set_height((int)Util::scale_value_by_resolution(32, 1080));
    } else if (index % beat_divisor == 0) {
        tick->set_height((int)Util::scale_value_by_resolution(16, 1080));
    } else {
        tick->set_height((int)Util::scale_value_by_resolution(8, 1080));
    }

    tick->set_opacity(.5f);
    tick->set_color(
        get_tick_color(beat_divisor, Util::mod(index, beat_divisor)));

    Vector2 pos;
    pos.x = tick_data["x"];
    pos.y = get_size().y - tick->get_height();
    tick->set_position(pos);
}

void ObjectTimeline::init_tick_color_schemes() {
    Color white = Color::hex(0xFFFFFFFF);
    Color red = Color::hex(0xF5130AFF);
    Color light_purple = Color::hex(0xC513BBFF);
    Color dark_purple = Color::hex(0x944C89FF);
    Color blue = Color::hex(0x4184EBFF);
    Color yellow = Color::hex(0xF5F40AFF);
    Color gray = Color::hex(0xA2A098FF);

    tick_color_schemes["1"] = Array::make(white);
    tick_color_schemes["2"] = Array::make(white, red);
    tick_color_schemes["3"] = Array::make(white, light_purple, dark_purple);
    tick_color_schemes["4"] = Array::make(white, blue, red, blue);
    tick_color_schemes["5"] =
        Array::make(white, yellow, yellow, yellow, yellow);
    tick_color_schemes["6"] = Array::make(white, dark_purple, light_purple, red,
                                          dark_purple, light_purple);
    tick_color_schemes["7"] =
        Array::make(white, yellow, yellow, yellow, yellow, yellow, yellow);
    tick_color_schemes["8"] =
        Array::make(white, yellow, blue, yellow, red, yellow, blue, yellow);
    tick_color_schemes["9"] =
        Array::make(white, yellow, yellow, light_purple, yellow, yellow,
                    light_purple, yellow, yellow);
    tick_color_schemes["12"] =
        Array::make(white, gray, dark_purple, blue, light_purple, gray, red,
                    gray, light_purple, blue, dark_purple, gray);
    tick_color_schemes["16"] =
        Array::make(white, gray, yellow, gray, blue, gray, yellow, gray, red,
                    gray, yellow, gray, blue, gray, yellow, gray);
}

Color ObjectTimeline::get_tick_color(int64_t beat_divisor, int64_t index) {
    String key = String::num_int64(beat_divisor);
    Array colors = tick_color_schemes[key];
    Color color = colors[(int)index];
    return color;
}

void ObjectTimeline::on_timeline_zoom_updated(float timeline_zoom) {
    draw_ticks(prev_percent_missing, prev_beat_number, prev_control_point);
}

void ObjectTimeline::set_hovering(bool hovering) { this->hovering = hovering; }

bool ObjectTimeline::is_hovering() { return hovering; }

void ObjectTimeline::on_mouse_entered() { hovering = true; }

void ObjectTimeline::on_mouse_exited() { hovering = false; }

void ObjectTimeline::on_timing_points_updated() {}

vector<Dictionary> ObjectTimeline::determine_timing_point_data() {
    vector<Dictionary> ret;

    return ret;
}