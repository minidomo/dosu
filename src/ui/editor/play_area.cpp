#include "./play_area.h"

#include <ResourceLoader.hpp>

#include "common/util.h"
#include "object/game/circle.h"
#include "singleton/map_manager.h"

void PlayArea::_register_methods() {
    dev_register_method(PlayArea, _ready);
    dev_register_method(PlayArea, on_song_position_updated);
}

void PlayArea::_init() {}

void PlayArea::_ready() {
    circle_container = get_node<Control>("HitObjects/Circles");

    circle_object = ResourceLoader::get_singleton()->load(
        "res://scenes/hit_object/Circle.tscn");
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

void PlayArea::draw_circle_objects(vector<HitObject *> hit_objects) {
    /*
    objects in the play area
    - objects at or after the song position
        - changing approach circle
        - if at song position
            - play hit sound
    - objects before the song position
        - are fading
    */
    int64_t diff = hit_objects.size() - circle_container->get_child_count();

    if (diff < 0) {
        // remove circles
        for (int i = 0; i < -diff; i++) {
            Node *child = circle_container->get_child(0);
            circle_container->remove_child(child);
            child->queue_free();
        }
    } else if (diff > 0) {
        // add circles
        for (int i = 0; i < diff; i++) {
            auto circle = Object::cast_to<Circle>(circle_object->instance());
            circle_container->add_child(circle);
        }
    }

    Array children = circle_container->get_children();
    for (int i = 0; i < children.size(); i++) {
        auto circle = Object::cast_to<Circle>(children[i]);
        setup_circle(circle, hit_objects[i]);
    }
}

void PlayArea::setup_circle(Circle *circle, HitObject *circle_data) {
    // adjust scale for cs
    // adjust scale for approach circle
    // - based on hit object position to song position
    // adjust modulate for fading out
    // - based on hit object position to song position
    auto map_manager = MapManager::get_singleton(this);

    int64_t song_position =
        Util::to_milliseconds(conductor->get_song_position());
    int64_t object_time = circle_data->get_start_time();

    auto beatmap = map_manager->get_editor_beatmap();
    int64_t fade_out_time = map_manager->get_fade_out_time();
    int64_t fade_in_time =
        map_manager->approach_rate_to_ms(beatmap->get_approach_rate());

    circle->set_approach_circle_visible(true);
    circle->set_circle_size(beatmap->get_circle_size());

    if (object_time < song_position) {
        // fading out, approach circle not touching but borders circle
        int64_t diff = std::abs(object_time - song_position);
        float percent = (float)diff / (float)fade_out_time;
        circle->set_opacity(percent);
    } else if (object_time > song_position) {
        // fade in, adjust approach circle
        int64_t diff = std::abs(object_time - song_position);
        float percent = 1.f - (float)diff / (float)fade_in_time;
        circle->set_opacity(percent);
    } else {
        // play hit sound (make sure this gets played only once), approach
        // circle touching border
        circle->set_opacity(1);
    }
}