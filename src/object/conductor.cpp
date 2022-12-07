#include "./conductor.h"

#include <AudioServer.hpp>
#include <AudioStream.hpp>
#include <Ref.hpp>

#include "common/util.h"

void Conductor::_register_methods() {
    dev_register_method(Conductor, _ready);
    dev_register_method(Conductor, _process);

    register_signal<Conductor>("song_position_updated", "song_position",
                               GODOT_VARIANT_TYPE_REAL);
}

void Conductor::_init() {
    song_position = 0;
    closest = 0;
    time_off_beat = 0;
}

void Conductor::_ready() {}

void Conductor::_process(float delta) {
    if (!is_actually_playing()) return;

    float time = calculate_song_position();

    if (time > song_position) {
        song_position = time;
        emit_signal("song_position_updated", song_position);
    }
}

float Conductor::calculate_song_position() {
    float time = get_playback_position() +
                 AudioServer::get_singleton()->get_time_since_last_mix();
    time -= AudioServer::get_singleton()->get_output_latency();
    time = Math::clamp<float>(time, 0, get_total_duration());
    return time;
}

float Conductor::get_total_duration() { return get_stream()->get_length(); }

float Conductor::get_song_position() { return song_position; }

/**
 * @return a Dictionary containing info about the requested beat
 *
 * time: float - the time of the beat
 * - index: int64_t - the beat number relative to the offset. the beat at the
 * offset starts at 0
 * - accessible_time: float - the time that is closest to the beat and within
 * the song duration
 * - accessible: bool - whether the beat is accessible. true if time ==
 * accessible_time, false otherwise
 * - step: float - the time between beats which is seconds_per_beat /
 * beat_divisor
 */
Dictionary Conductor::get_beat(float position, float song_offset,
                               int64_t beat_offset, int64_t beat_divisor,
                               float bpm) {
    dev_assert(beat_offset != 0);

    float time = song_offset;
    float seconds_per_beat = 60 / bpm;
    float step = seconds_per_beat / beat_divisor;
    int64_t index = 0;

    if (Util::float_gte(position, song_offset)) {
        bool on_beat = false;

        // time will be on the beat after the position
        while (Util::float_lte(time, position)) {
            on_beat = Math::is_equal_approx(time, position);
            time += step;
            index++;
        }

        if (beat_offset > 0) {
            // get a beat ahead the current position

            for (int64_t beat = 1; beat < beat_offset; beat++) {
                time += step;
                index++;
            }
        } else {
            // get a beat behind the current position

            /*
            the while loop above always gets the next beat from the position.

            if the given position is in between beats, i.e. not on a beat
            itself, then we can get the next beat and go backwards one to get
            the previous beat.

            if the given position is on a beat, then we have to
            go backwards two to get the previous beat.
            */

            for (int64_t beat = on_beat; beat > beat_offset; beat--) {
                time -= step;
                index--;
            }
        }

    } else {
        // the song offset can be ahead of the posiion in cases where the song
        // starts later and the user is at the beginning of song

        bool on_beat = false;

        // time will be on the beat before the position
        while (Util::float_gte(time, position)) {
            on_beat = Math::is_equal_approx(time, position);
            time -= step;
            index--;
        }

        if (beat_offset > 0) {
            // get a beat ahead the current position

            for (int64_t beat = -1 * (int64_t)on_beat; beat < beat_offset;
                 beat++) {
                time += step;
                index++;
            }
        } else {
            // get a beat behind the current position

            for (int64_t beat = -1; beat > beat_offset; beat--) {
                time -= step;
                index--;
            }
        }
    }

    float accessible_time = Math::clamp<float>(time, 0, get_total_duration());
    bool accessible = Math::is_equal_approx(accessible_time, time);

    Dictionary ret;
    ret["time"] = time;
    ret["index"] = index;
    ret["accessible_time"] = accessible_time;
    ret["accessible"] = accessible;
    ret["step"] = step;

    return ret;
}

void Conductor::toggle_pause() {
    if (is_playing()) {
        set_stream_paused(!get_stream_paused());
    } else {
        float target = song_position;
        if (Math::is_equal_approx(target, get_total_duration())) {
            target = 0;
        }

        go_to(target, ConductorGoType::Play);
    }
}

void Conductor::go_to(float time, ConductorGoType action) {
    song_position = time;
    emit_signal("song_position_updated", song_position);

    switch (action) {
        case ConductorGoType::Play: {
            set_stream_paused(false);
            play(song_position);
            break;
        }
        case ConductorGoType::Pause: {
            if (is_playing()) {
                set_stream_paused(true);
                seek(song_position);
            }
            break;
        }
        case ConductorGoType::Maintain: {
            seek(song_position);
            break;
        }
        default: {
            dev_assert(false);
            break;
        }
    }
}

void Conductor::go_to_percent(float percent, ConductorGoType action) {
    float time = percent * get_total_duration();
    go_to(time, action);
}

bool Conductor::is_actually_playing() {
    return is_playing() && !get_stream_paused();
}