#include "./timing_point.h"

TimingPoint::TimingPoint() {}

TimingPoint::~TimingPoint() {}

void TimingPoint::parse_line(String line) {
    // https://github.com/kionell/osu-parsers/blob/32fc96237dec238d278aa22c15a31a3bf5b7c8d2/src/core/Decoders/Handlers/Beatmaps/BeatmapTimingPointDecoder.ts
    // Time,beatLength,meter,sampleSet,sampleIndex,volume,uninherited,effects

    auto data = line.split(",");
    time = data[0].to_int();
    beat_length = data[1].to_float();
    meter = data[2].to_int();
    uninherited = data[6].to_int() == 1;

    if (uninherited) {
        /* red lines */
        bpm = calculate_bpm(beat_length);
    } else {
        /* green lines */
        slider_velocity = 100 / -beat_length;
        bpm_multiplier = Math::min(Math::floor(-beat_length), 10000.f);
        bpm_multiplier = Math::max(10.f, bpm_multiplier) / 100;
    }
}

String TimingPoint::to_file_string() {
    String ret = String::num_int64(time);

    ret += "," + String::num_real(beat_length);
    ret += "," + String::num_int64(meter);
    ret += ",2,1,60";
    ret += "," + String::num_int64(uninherited);
    ret += ",0";

    return ret;
}

void TimingPoint::init_red_line() {
    set_time(0);
    set_meter(4);
    set_uninherited(true);
    set_bpm(100);
}

void TimingPoint::set_time(int64_t time) { this->time = time; }

void TimingPoint::set_beat_length(float beat_length) {
    this->beat_length = beat_length;
    bpm = calculate_bpm(beat_length);
}

void TimingPoint::set_meter(int64_t meter) { this->meter = meter; }

void TimingPoint::set_uninherited(bool uninherited) {
    this->uninherited = uninherited;
}

void TimingPoint::set_bpm(float bpm) {
    this->bpm = bpm;
    beat_length = calculate_beat_length(bpm);
}

float TimingPoint::calculate_bpm(float beat_length) {
    return 60000 / beat_length;
}

float TimingPoint::calculate_beat_length(float bpm) { return 60000 / bpm; }

void TimingPoint::copy(TimingPoint timing_point) {
    time = timing_point.time;
    beat_length = timing_point.beat_length;
    meter = timing_point.meter;
    uninherited = timing_point.uninherited;
    bpm_multiplier = timing_point.bpm_multiplier;
    slider_velocity = timing_point.slider_velocity;
    bpm = timing_point.bpm;
}