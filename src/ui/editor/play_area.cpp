#include "./play_area.h"

#include <ResourceLoader.hpp>

#include "common/util.h"
#include "singleton/map_manager.h"

void PlayArea::_register_methods() {
    dev_register_method(PlayArea, _ready);
    dev_register_method(PlayArea, on_song_position_updated);
    dev_register_method(PlayArea, on_approach_rate_updated);
    dev_register_method(PlayArea, on_circle_size_updated);
    dev_register_method(PlayArea, on_hit_objects_updated);
}

void PlayArea::_init() {}

void PlayArea::_ready() {
    circle_container = get_node<Control>("HitObjects/Circles");

    circle_object = ResourceLoader::get_singleton()->load(
        "res://scenes/hit_object/Circle.tscn");

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->connect("approach_rate_updated", this, "on_approach_rate_updated");
    beatmap->connect("circle_size_updated", this, "on_circle_size_updated");
    beatmap->connect("hit_objects_updated", this, "on_hit_objects_updated");
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
    auto map_manager = MapManager::get_singleton(this);
    auto beatmap = map_manager->get_editor_beatmap();

    int64_t mid_point = Util::to_milliseconds(song_position);
    int64_t start_time = mid_point - map_manager->get_fade_out_time();
    int64_t end_time = mid_point + map_manager->approach_rate_to_ms(
                                       beatmap->get_approach_rate());

    update_played_hit_sounds(mid_point);

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
    auto beatmap = map_manager->get_editor_beatmap();

    circle->set_approach_circle_visible(true);
    circle->set_circle_size(beatmap->get_circle_size());

    Color color =
        circle_colors[String::num_int64(circle_data->get_start_time())];
    circle->set_color(color);

    Vector2 pos;
    pos.x = (float)circle_data->get_start_x();
    pos.y = (float)circle_data->get_start_y();
    circle->set_position(convert_to_external(pos));

    int64_t song_position =
        Util::to_milliseconds(conductor->get_song_position());
    int64_t start_time = circle_data->get_start_time();

    int64_t fade_out_time = map_manager->get_fade_out_time();
    int64_t fade_in_time =
        map_manager->approach_rate_to_ms(beatmap->get_approach_rate());

    int64_t diff = start_time - song_position;
    int64_t abs_diff = std::abs(diff);

    if (can_play_hit_sound(start_time, song_position)) {
        play_hit_sound(circle, start_time);
    }

    if (start_time < song_position) {
        // fading out, approach circle not touching but borders circle
        float percent = 1.f - (float)abs_diff / fade_out_time;
        circle->set_opacity(percent);
        circle->approach_circle_standard_position();
    } else if (start_time > song_position) {
        // fade in, adjust approach circle
        float percent = 1.f - (float)abs_diff / fade_in_time;
        circle->set_opacity(percent);
        circle->set_approach_circle_progress(percent);
    } else {
        circle->set_opacity(1);
        circle->set_approach_circle_progress(1);
    }
}

void PlayArea::on_approach_rate_updated(float approach_rate) {
    on_song_position_updated(conductor->get_song_position());
}

void PlayArea::on_circle_size_updated(float circle_size) {
    on_song_position_updated(conductor->get_song_position());
}

bool PlayArea::can_play_hit_sound(int64_t hit_sound_time, int64_t song_time) {
    if (!is_visible_in_tree()) return false;
    if (!conductor->is_actually_playing()) return false;

    int64_t diff = hit_sound_time - song_time;

    if (-5 <= diff && diff <= 5) {
        String key = String::num_int64(hit_sound_time);
        return !played_hit_sounds.has(key);
    }

    return false;
}

void PlayArea::play_hit_sound(Circle *circle, int64_t hit_sound_time) {
    String key = String::num_int64(hit_sound_time);
    played_hit_sounds[key] = true;
    circle->play_hit_sound();
}

void PlayArea::update_played_hit_sounds(int64_t song_time) {
    Array keys = played_hit_sounds.keys();

    for (int i = 0; i < keys.size(); i++) {
        String key = keys[i];
        int64_t time = key.to_int();

        if (time >= song_time) {
            played_hit_sounds.erase(key);
        }
    }
}

void PlayArea::on_hit_objects_updated() {
    update_circle_colors();
    on_song_position_updated(conductor->get_song_position());
}

void PlayArea::update_circle_colors() {
    circle_colors.clear();

    auto map_manager = MapManager::get_singleton(this);
    auto colors = map_manager->get_taiko_colors();
    auto beatmap = map_manager->get_editor_beatmap();
    auto hit_objects = beatmap->get_hit_objects();

    int64_t color_index = 1;
    int64_t prev_hit_sound = -1;
    for (int i = 0; i < hit_objects.size(); i++) {
        auto hit_object = hit_objects[i];

        if (prev_hit_sound != hit_object->get_hit_sound()) {
            prev_hit_sound = hit_object->get_hit_sound();
            color_index = (color_index + 1) % colors.size();
        }

        String color_key = String::num_int64(color_index);
        Color color = colors[color_key];

        String key = String::num_int64(hit_object->get_start_time());
        circle_colors[key] = color;
    }
}

void PlayArea::initialize() {
    update_circle_colors();
    on_song_position_updated(conductor->get_song_position());
}
