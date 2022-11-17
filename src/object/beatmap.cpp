#include "./beatmap.h"

Beatmap::Beatmap() {}

Beatmap::~Beatmap() {}

void Beatmap::init() {
    /* general */
    audio_filename = "audio.mp3";
    preview_time = 0;

    /* editor */
    beat_divisor = 4;
    timeline_zoom = 1;

    /* metadata */
    title = "unknown";
    title_unicode = "unknown";
    artist = "unknown";
    artist_unicode = "unknown";
    creator = "dosu user";
    version = "unknown";
    source;
    tags;
    beatmap_id;
    beatmap_set_id;

    /* difficulty */
    hp_drain_rate = 6;
    circle_size = 4;
    overall_difficulty = 8;
    approach_rate = 9;
    slider_multiplier = 1;
    slider_tick_rate = 1;

    /* events */
    background_filename;
    break_periods;

    /* timing points */
    timing_points;

    /* hit objects */
    hit_objects;
}

void Beatmap::parse_contents(File *file) {}

void Beatmap::write_contents(File *file) {
    file->store_line("osu file format v14");
    file->store_line("");

    /* general */
    file->store_line("[General]");
    file->store_line("AudioFilename: " + audio_filename);
    file->store_line("AudioLeadIn: 0");
    file->store_line("PreviewTime: " + String::num_int64(preview_time));
    file->store_line("Countdown: 0");
    file->store_line("SampleSet: Soft");
    file->store_line("StackLeniency: 0.7");
    file->store_line("Mode: 0");
    file->store_line("LetterboxInBreaks: 0");
    file->store_line("WidescreenStoryboard: 1");
    file->store_line("");

    /* editor */
    file->store_line("[Editor]");
    file->store_line("DistanceSpacing: 1");
    file->store_line("BeatDivisor: " + String::num_int64(beat_divisor));
    file->store_line("GridSize: 32");
    file->store_line("TimelineZoom: " + String::num_real(timeline_zoom));
    file->store_line("");

    /* metadata */
    file->store_line("[Metadata]");
    file->store_line("Title:" + title);
    file->store_line("TitleUnicode:" + title_unicode);
    file->store_line("Artist:" + artist);
    file->store_line("ArtistUnicode:" + artist_unicode);
    file->store_line("Creator:" + creator);
    file->store_line("Version:" + version);
    file->store_line("Source:" + source);
    file->store_line("Tags:" + to_string_tags());
    file->store_line("BeatmapID:0");
    file->store_line("BeatmapSetID:-1");
    file->store_line("");

    /* difficulty */
    file->store_line("[Difficulty]");
    file->store_line("HPDrainRate:" + String::num_real(hp_drain_rate));
    file->store_line("CircleSize:" + String::num_real(circle_size));
    file->store_line("OverallDifficulty:" +
                     String::num_real(overall_difficulty));
    file->store_line("ApproachRate:" + String::num_real(approach_rate));
    file->store_line("SliderMultiplier:" + String::num_real(slider_multiplier));
    file->store_line("SliderTickRate:" + String::num_real(slider_tick_rate));
    file->store_line("");

    /* events */
    file->store_line("[Events]");

    file->store_line("//Background and Video events");
    if (!background_filename.empty()) {
        file->store_line(to_string_background());
    }

    file->store_line("//Break Periods");
    for (auto bp : break_periods) {
        file->store_line(bp.to_file_string());
    }

    file->store_line("//Storyboard Layer 0 (Background)");
    file->store_line("//Storyboard Layer 1 (Fail)");
    file->store_line("//Storyboard Layer 2 (Pass)");
    file->store_line("//Storyboard Layer 3 (Foreground)");
    file->store_line("//Storyboard Layer 4 (Overlay)");
    file->store_line("//Storyboard Sound Samples");
    file->store_line("");

    /* timing points */
    file->store_line("[TimingPoints]");
    for (auto tp : timing_points) {
        file->store_line(tp.to_file_string());
    }
    file->store_line("");

    /* colors - ignoring */
    // file->store_line("[Colours]");
    // file->store_line("");

    /* hit objects */
    file->store_line("[HitObjects]");
    for (auto ho : hit_objects) {
        file->store_line(ho.to_file_string());
    }
    file->store_line("");
}

String Beatmap::to_string_tags() {
    String ret = "";

    if (tags.size() > 0) {
        ret = tags[0];
    }

    for (int i = 1; i < tags.size(); i++) {
        ret += " " + tags[i];
    }

    return ret;
}

String Beatmap::to_string_background() {
    return "0,0,\"" + background_filename + "\",0,0";
}
