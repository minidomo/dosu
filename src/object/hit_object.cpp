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

    if (hit_type & +HitType::Spinner) {
        end_time = extras[0].to_int();
    }

    if (hit_type & +HitType::Slider) {
    }
}

String HitObject::to_file_string() {
    String ret = String::num_int64(start_x);
    ret += "," + String::num_int64(start_y);
    ret += "," + String::num_int64(start_time);
    ret += "," + String::num_int64(hit_type);

    if (hit_type & +HitType::Spinner) {
        ret += "," + String::num_int64(end_time);
    } else if (hit_type & +HitType::Slider) {
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