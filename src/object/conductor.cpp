#include "./conductor.h"

#include <AudioServer.hpp>
#include <AudioStream.hpp>
#include <Ref.hpp>

#include "common/util.h"

void Conductor::_register_methods() {
    register_method("_ready", &Conductor::_ready);
    register_method("_physics_process", &Conductor::_physics_process);
    register_method("on_timeout", &Conductor::on_timeout);

    register_signal<Conductor>("beat", "beat", GODOT_VARIANT_TYPE_INT);
    register_signal<Conductor>("measure", "measure", GODOT_VARIANT_TYPE_INT);
    register_signal<Conductor>("song_position_update", "song_position",
                               GODOT_VARIANT_TYPE_INT);
}

void Conductor::_init() {
    bpm = 100;
    measures = 4;

    song_position = 0;
    song_position_in_beats = 1;
    sec_per_beat = 60.f / bpm;
    last_reported_beat = 0;
    beats_before_start = 0;
    measure = 0;

    closest = 0;
    time_off_beat = 0;
}

void Conductor::_ready() {
    start_timer = get_node<Timer>("StartTimer");
    start_timer->connect("timeout", this, "on_timeout");
}

void Conductor::_physics_process(real_t delta) {
    if (is_playing() && !get_stream_paused()) {
        song_position = get_playback_position() +
                        AudioServer::get_singleton()->get_time_since_last_mix();
        song_position -= AudioServer::get_singleton()->get_output_latency();
        emit_signal("song_position_update",
                    Util::to_milliseconds(song_position));

        song_position_in_beats =
            (int64_t)Math::floor(song_position / sec_per_beat) +
            beats_before_start;
        report_beat();
    }
}

void Conductor::report_beat() {
    if (last_reported_beat < song_position_in_beats) {
        if (measure > measures) {
            measure = 1;
        }
        emit_signal("beat", song_position_in_beats);
        emit_signal("measure", measure);
        last_reported_beat = song_position_in_beats;
        measure++;
    }
}

void Conductor::play_with_beat_offset(int64_t beats) {
    beats_before_start = beats;
    start_timer->set_wait_time(sec_per_beat);
    start_timer->start();
}

pair<int64_t, float> Conductor::closest_beat(float nth) {
    closest =
        (int64_t)(Math::round((song_position / sec_per_beat) / nth) * nth);

    time_off_beat = closest * sec_per_beat - song_position;
    if (time_off_beat < 0) {
        time_off_beat = -time_off_beat;
    }

    return {closest, time_off_beat};
}

void Conductor::play_from_beat(int64_t beat, int64_t offset) {
    play();
    seek(beat * sec_per_beat);
    beats_before_start = offset;
    measure = beat % measures;
}

void Conductor::on_timeout() {
    song_position_in_beats++;

    if (song_position_in_beats < beats_before_start - 1) {
        start_timer->start();
    } else if (song_position_in_beats == beats_before_start - 1) {
        auto audio_server = AudioServer::get_singleton();
        float wait_time = start_timer->get_wait_time() -
                          audio_server->get_time_to_next_mix() -
                          audio_server->get_output_latency();
        start_timer->set_wait_time(wait_time);
        start_timer->start();
    } else {
        play();
        start_timer->stop();
    }

    report_beat();
}

void Conductor::set_bpm(float bpm) {
    this->bpm = bpm;
    sec_per_beat = 60.f / bpm;
}

void Conductor::set_measures(int64_t measures) { this->measures = measures; }

float Conductor::get_bpm() { return bpm; }

int64_t Conductor::get_measures() { return measures; }

int64_t Conductor::get_total_duration() {
    return Util::to_milliseconds(get_stream()->get_length());
}

int64_t Conductor::get_song_position() {
    return Util::to_milliseconds(song_position);
}

void Conductor::toggle_pause() {
    if (is_playing()) {
        set_stream_paused(!get_stream_paused());
    } else {
        go_to(0, ConductorGoType::Play);
    }
}

void Conductor::go_to(int64_t ms, ConductorGoType action) {
    song_position = Util::to_seconds(ms);
    emit_signal("song_position_update", ms);

    switch (action) {
        case ConductorGoType::Play: {
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
