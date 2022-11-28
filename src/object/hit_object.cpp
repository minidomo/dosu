#include "./hit_object.h"

#include "enum/hit_type.h"

void HitObject::_register_methods() {}

void HitObject::_init() {}

void HitObject::parse_line(String line) {
    // https://github.com/kionell/osu-parsers/blob/32fc96237dec238d278aa22c15a31a3bf5b7c8d2/src/core/Decoders/Handlers/Beatmaps/BeatmapHitObjectDecoder.ts
    // x,y,time,type,hitSound,objectParams,hitSample

    auto data = line.split(",");
    for (int i = 0; i < data.size(); i++) {
        data[i].trim_prefix(" ");
        data[i].trim_suffix(" ");
    }

    PoolStringArray extras;
    for (int i = 5; i < data.size(); i++) {
        extras.append(data[i]);
    }

    start_x = data[0].to_int();
    start_y = data[1].to_int();
    start_time = data[2].to_int();
    new_combo = !!(hit_type & +HitType::NewCombo);
    hit_sound = data[4].to_int();

    if (is_spinner()) {
        end_time = extras[0].to_int();
    }

    if (is_slider()) {
    }
}

String HitObject::to_file_string() {
    String ret = String::num_int64(start_x);
    ret += "," + String::num_int64(start_y);
    ret += "," + String::num_int64(start_time);
    ret += "," + String::num_int64(hit_type);

    if (is_spinner()) {
        ret += "," + String::num_int64(end_time);
    } else if (is_slider()) {
        // TODO
    }

    ret += ",0:0:0:0:";

    return ret;
}

void HitObject::copy(HitObject *hit_object) {
    hit_type = hit_object->hit_type;
    start_x = hit_object->start_x;
    start_y = hit_object->start_y;
    start_time = hit_object->start_time;
    new_combo = hit_object->new_combo;
    end_time = hit_object->end_time;
}

void HitObject::set_hit_type(int64_t hit_type) { this->hit_type = hit_type; }

void HitObject::set_start_x(int64_t start_x) { this->start_x = start_x; }

void HitObject::set_start_y(int64_t start_y) { this->start_y = start_y; }

void HitObject::set_start_time(int64_t start_time) {
    this->start_time = start_time;
}

void HitObject::set_hit_sound(int64_t hit_sound) {
    this->hit_sound = hit_sound;
}

void HitObject::set_new_combo(bool new_combo) { this->new_combo = new_combo; }

void HitObject::set_end_time(int64_t end_time) { this->end_time = end_time; }

int64_t HitObject::get_hit_type() { return hit_type; }

int64_t HitObject::get_start_x() { return start_x; }

int64_t HitObject::get_start_y() { return start_y; }

int64_t HitObject::get_start_time() { return start_time; }

int64_t HitObject::get_hit_sound() { return hit_sound; }

bool HitObject::is_new_combo() { return new_combo; }

int64_t HitObject::get_end_time() { return end_time; }

bool HitObject::is_normal() { return !!(hit_type & +HitType::Normal); }

bool HitObject::is_spinner() { return !!(hit_type & +HitType::Spinner); }

bool HitObject::is_slider() { return !!(hit_type & +HitType::Slider); }
