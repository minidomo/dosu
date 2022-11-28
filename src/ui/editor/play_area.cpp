#include "./play_area.h"

#include "common/util.h"
#include "singleton/map_manager.h"

void PlayArea::_register_methods() {
    dev_register_method(PlayArea, _ready);
    dev_register_method(PlayArea, on_song_position_updated);
}

void PlayArea::_init() {}

void PlayArea::_ready() {
    circle_container = get_node<Control>("HitObjects/Circles");
}

Vector2 PlayArea::convert_to_internal(Vector2 external_coordinate) {
    Vector2 dimensions = get_size();

    Vector2 ret;

    ret.x = external_coordinate.x / dimensions.x *
            MapManager::get_singleton(this)->get_max_placeable_x_coordinate();
    ret.y = external_coordinate.y / dimensions.y *
            MapManager::get_singleton(this)->get_max_placeable_y_coordinate();

    ret.x = Math::floor(ret.x);
    ret.y = Math::floor(ret.y);

    return ret;
}

Vector2 PlayArea::convert_to_external(Vector2 internal_coordinate) {
    Vector2 dimensions = get_size();

    Vector2 ret;

    ret.x = internal_coordinate.x /
            MapManager::get_singleton(this)->get_max_placeable_x_coordinate() *
            dimensions.x;
    ret.y = internal_coordinate.y /
            MapManager::get_singleton(this)->get_max_placeable_y_coordinate() *
            dimensions.y;

    return ret;
}

void PlayArea::set_conductor(Conductor *conductor) {
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

Conductor *PlayArea::get_conductor() { return conductor; }

void PlayArea::on_song_position_updated(float song_position) {
    Godot::print("play area got song position: " +
                 String::num_int64(Util::to_milliseconds(song_position)));

    auto map_manager = MapManager::get_singleton(this);
    auto beatmap = map_manager->get_editor_beatmap();

    int64_t start_time = Util::to_milliseconds(song_position);
    int64_t end_time = start_time + map_manager->approach_rate_to_ms(
                                        beatmap->get_approach_rate());

    auto hit_objects = beatmap->find_hit_objects(start_time, end_time);
    draw_hit_objects(hit_objects);
}

void PlayArea::draw_hit_objects(vector<HitObject *> hit_objects) {
    vector<HitObject *> circle_objects;

    for (auto ho : hit_objects) {
        if (ho->is_normal()) {
            circle_objects.push_back(ho);
        }
    }

    draw_circle_objects(circle_objects);
}

void PlayArea::draw_circle_objects(vector<HitObject *> hit_objects) {}