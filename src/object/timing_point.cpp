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

    slider_velocity = 100 / -beat_length;
    bpm_multiplier = Math::min(Math::floor(-beat_length), 10000.f);
    bpm_multiplier = Math::max(10.f, bpm_multiplier) / 100;
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
