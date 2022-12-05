#include "./timeline.h"

#include <InputEventMouseButton.hpp>
#include <ResourceLoader.hpp>

#include "common/util.h"
#include "singleton/map_manager.h"

void Timeline::_register_methods() {
    dev_register_method(Timeline, _ready);
    dev_register_method(Timeline, _gui_input);
    dev_register_method(Timeline, on_timing_points_updated);
    dev_register_method(Timeline, on_song_position_updated);

    register_signal<Timeline>("timeline_click", "percent",
                              GODOT_VARIANT_TYPE_REAL);
}

void Timeline::_init() {}

void Timeline::_ready() {
    playhead = get_node<ColorRect>("Playhead");
    timing_points = get_node<Control>("TimingPoints");

    tick_object =
        ResourceLoader::get_singleton()->load("res://scenes/editor/Tick.tscn");

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->connect("timing_points_updated", this, "on_timing_points_updated");
}

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

void Timeline::on_song_position_updated(float song_position) {
    float percent = song_position / conductor->get_total_duration();
    set_playhead_progress(percent);
}

void Timeline::on_timing_points_updated() {
    auto timing_point_data = determine_timing_point_data();
    int64_t diff = timing_point_data.size() - timing_points->get_child_count();

    if (diff < 0) {
        // delete timing points
        for (int i = 0; i < -diff; i++) {
            Node *child = timing_points->get_child(0);
            timing_points->remove_child(child);
            child->queue_free();
        }
    } else if (diff > 0) {
        // add timing points
        for (int i = 0; i < diff; i++) {
            auto tick = Object::cast_to<Tick>(tick_object->instance());
            timing_points->add_child(tick);
        }
    }

    Array children = timing_points->get_children();
    for (int i = 0; i < children.size(); i++) {
        auto tick = Object::cast_to<Tick>(children[i]);
        setup_timing_point(tick, timing_point_data[i]);
    }
}

void Timeline::set_conductor(Conductor *conductor) {
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

Conductor *Timeline::get_conductor() { return conductor; }

void Timeline::initialize() {
    on_timing_points_updated();
    on_song_position_updated(conductor->get_song_position());
}

/**
 * @return vector of Dictionaries containing the data of timing points for the
 * timeline.
 * x: int64_t - the x position of the timing point
 * color: Color - the color of the timing point
 */
vector<Dictionary> Timeline::determine_timing_point_data() {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    auto timing_points = beatmap->get_timing_points();

    vector<Dictionary> ret;

    for (auto tp : timing_points) {
        Dictionary data;

        float x = Util::to_seconds(tp->get_time()) /
                  conductor->get_total_duration() * get_size().x;
        data["x"] = x;
        data["color"] = tp->get_color();

        ret.push_back(data);
    }

    return ret;
}

void Timeline::setup_timing_point(Tick *timing_point,
                                  Dictionary timing_point_data) {
    Color color = timing_point_data["color"];
    timing_point->set_opacity(.6f);
    timing_point->set_color(color);

    Vector2 pos;
    pos.x = timing_point_data["x"];
    pos.y = 0;

    timing_point->set_height((int)timing_points->get_size().height);
    timing_point->set_position(pos);
}