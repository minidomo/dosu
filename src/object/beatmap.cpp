#include "./beatmap.h"

#include "common/util.h"
#include "singleton/game.h"
#include "singleton/map_manager.h"

void Beatmap::_register_methods() {
    register_signal<Beatmap>("timeline_zoom_updated", "value",
                             GODOT_VARIANT_TYPE_REAL);

    register_signal<Beatmap>("title_updated", "value",
                             GODOT_VARIANT_TYPE_STRING);
    register_signal<Beatmap>("title_unicode_updated", "value",
                             GODOT_VARIANT_TYPE_STRING);
    register_signal<Beatmap>("artist_updated", "value",
                             GODOT_VARIANT_TYPE_STRING);
    register_signal<Beatmap>("artist_unicode_updated", "value",
                             GODOT_VARIANT_TYPE_STRING);
    register_signal<Beatmap>("creator_updated", "value",
                             GODOT_VARIANT_TYPE_STRING);
    register_signal<Beatmap>("version_updated", "value",
                             GODOT_VARIANT_TYPE_STRING);
    register_signal<Beatmap>("source_updated", "value",
                             GODOT_VARIANT_TYPE_STRING);
    register_signal<Beatmap>("tags_updated", "value",
                             GODOT_VARIANT_TYPE_POOL_STRING_ARRAY);

    register_signal<Beatmap>("hp_drain_rate_updated", "value",
                             GODOT_VARIANT_TYPE_INT);
    register_signal<Beatmap>("circle_size_updated", "value",
                             GODOT_VARIANT_TYPE_INT);
    register_signal<Beatmap>("overall_difficulty_updated", "value",
                             GODOT_VARIANT_TYPE_INT);
    register_signal<Beatmap>("approach_rate_updated", "value",
                             GODOT_VARIANT_TYPE_INT);

    register_signal<Beatmap>("background_filename_updated", "value",
                             GODOT_VARIANT_TYPE_STRING);
}

void Beatmap::_init() {}

void Beatmap::initialize(String beatmap_set_id, String beatmap_id) {
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
    this->beatmap_id = beatmap_id;
    this->beatmap_set_id = beatmap_set_id;

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
    auto first_tp = TimingPoint::_new();
    first_tp->init_red_line();
    timing_points.push_back(first_tp);

    /* hit objects */
    hit_objects;
}

void Beatmap::parse_contents(File *file) {
    PoolStringArray lines = get_lines(file);
    parse_general_section(lines);
    parse_editor_section(lines);
    parse_metadata_section(lines);
    parse_difficulty_section(lines);
    parse_events_section(lines);
    parse_timing_points(lines);
    parse_hit_objects_section(lines);
}

void Beatmap::parse_general_section(PoolStringArray lines) {
    int origin = find_line_index(lines, "[General]");
    dev_assert(origin >= 0);

    audio_filename = get_right_value(lines[origin + 1], "AudioFilename: ");
    preview_time = get_right_value(lines[origin + 3], "PreviewTime: ").to_int();
}

void Beatmap::parse_editor_section(PoolStringArray lines) {
    int origin = find_line_index(lines, "[Editor]");
    dev_assert(origin >= 0);

    beat_divisor = get_right_value(lines[origin + 2], "BeatDivisor: ").to_int();
    timeline_zoom =
        get_right_value(lines[origin + 4], "TimelineZoom: ").to_float();
}

void Beatmap::parse_metadata_section(PoolStringArray lines) {
    int origin = find_line_index(lines, "[Metadata]");
    dev_assert(origin >= 0);

    title = get_right_value(lines[origin + 1], "Title:");
    title_unicode = get_right_value(lines[origin + 2], "TitleUnicode:");
    artist = get_right_value(lines[origin + 3], "Artist:");
    artist_unicode = get_right_value(lines[origin + 4], "ArtistUnicode:");
    creator = get_right_value(lines[origin + 5], "Creator:");
    version = get_right_value(lines[origin + 6], "Version:");
    source = get_right_value(lines[origin + 7], "Source:");
    tags = get_right_value(lines[origin + 8], "Tags:").split(" ");
    beatmap_id = get_right_value(lines[origin + 9], "BeatmapID:");
    beatmap_set_id = get_right_value(lines[origin + 10], "BeatmapSetID:");
}

void Beatmap::parse_difficulty_section(PoolStringArray lines) {
    int origin = find_line_index(lines, "[Difficulty]");
    dev_assert(origin >= 0);

    hp_drain_rate =
        get_right_value(lines[origin + 1], "HPDrainRate:").to_float();
    circle_size = get_right_value(lines[origin + 2], "CircleSize:").to_float();
    overall_difficulty =
        get_right_value(lines[origin + 3], "OverallDifficulty:").to_float();
    approach_rate =
        get_right_value(lines[origin + 4], "ApproachRate:").to_float();
    slider_multiplier =
        get_right_value(lines[origin + 5], "SliderMultiplier:").to_float();
    slider_tick_rate =
        get_right_value(lines[origin + 6], "SliderTickRate:").to_float();
}

void Beatmap::parse_events_section(PoolStringArray lines) {
    int bg_index = find_line_index(lines, "//Background and Video events");
    dev_assert(bg_index >= 0);

    int bp_index = find_line_index(lines, "//Break Periods");
    dev_assert(bp_index >= 0);

    int sbbg_index =
        find_line_index(lines, "//Storyboard Layer 0 (Background)");
    dev_assert(sbbg_index >= 0);

    // check if there's a background
    if (bp_index - bg_index > 1) {
        String line = lines[bg_index + 1];
        line = line.trim_prefix("0,0,\"");
        line = line.trim_suffix("\",0,0");
        background_filename = line;
    }

    for (int i = bp_index + 1; i < sbbg_index; i++) {
        auto bp = BreakPeriod::_new();
        bp->parse_line(lines[i]);
        break_periods.push_back(bp);
    }
}

void Beatmap::parse_timing_points(PoolStringArray lines) {
    int origin = find_line_index(lines, "[TimingPoints]");
    dev_assert(origin >= 0);

    for (int i = origin + 1; lines[i] != "[HitObjects]"; i++) {
        auto tp = TimingPoint::_new();
        tp->parse_line(lines[i]);
        timing_points.push_back(tp);
    }
}

void Beatmap::parse_hit_objects_section(PoolStringArray lines) {
    int origin = find_line_index(lines, "[HitObjects]");
    dev_assert(origin >= 0);

    for (int i = origin + 1; i < lines.size(); i++) {
        auto ho = HitObject::_new();
        ho->parse_line(lines[i]);
        hit_objects.push_back(ho);
    }
}

String Beatmap::get_right_value(String line, String prefix) {
    return line.right(prefix.length());
}

PoolStringArray Beatmap::get_lines(File *file) {
    String content = file->get_as_text();
    PoolStringArray lines = content.split("\n");

    for (int i = 0; i < lines.size(); i++) {
        lines[0].strip_edges();
    }

    return lines;
}

int Beatmap::find_line_index(PoolStringArray lines, String section) {
    for (int i = 0; i < lines.size(); i++) {
        if (lines[i] == section) {
            return i;
        }
    }

    return -1;
}

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
    file->store_line("Mode: 1");
    file->store_line("LetterboxInBreaks: 0");
    file->store_line("WidescreenStoryboard: 1");
    file->store_line("");

    /* editor */
    file->store_line("[Editor]");
    file->store_line("DistanceSpacing: 1");
    file->store_line("BeatDivisor: " + String::num_int64(beat_divisor));
    file->store_line("GridSize: 32");
    file->store_line("TimelineZoom: " + Util::limit_decimal(timeline_zoom, 1));
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
    file->store_line("BeatmapID:" + beatmap_id);
    file->store_line("BeatmapSetID:" + beatmap_set_id);
    file->store_line("");

    /* difficulty */
    file->store_line("[Difficulty]");
    file->store_line("HPDrainRate:" + Util::limit_decimal(hp_drain_rate, 1));
    file->store_line("CircleSize:" + Util::limit_decimal(circle_size, 1));
    file->store_line("OverallDifficulty:" +
                     Util::limit_decimal(overall_difficulty, 1));
    file->store_line("ApproachRate:" + Util::limit_decimal(approach_rate, 1));
    file->store_line("SliderMultiplier:" +
                     Util::limit_decimal(slider_multiplier, 1));
    file->store_line("SliderTickRate:" +
                     Util::limit_decimal(slider_tick_rate, 1));
    file->store_line("");

    /* events */
    file->store_line("[Events]");

    file->store_line("//Background and Video events");
    if (!background_filename.empty()) {
        file->store_line(to_string_background());
    }

    file->store_line("//Break Periods");
    for (auto bp : break_periods) {
        file->store_line(bp->to_file_string());
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
        file->store_line(tp->to_file_string());
    }
    file->store_line("");

    /* colors - ignoring */
    // file->store_line("[Colours]");
    // file->store_line("");

    /* hit objects */
    file->store_line("[HitObjects]");
    for (auto ho : hit_objects) {
        file->store_line(ho->to_file_string());
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

String Beatmap::get_audio_filename() { return audio_filename; }

int64_t Beatmap::get_preview_time() { return preview_time; }

int64_t Beatmap::get_beat_divisor() { return beat_divisor; }

float Beatmap::get_timeline_zoom() { return timeline_zoom; }

String Beatmap::get_title() { return title; }

String Beatmap::get_title_unicode() { return title_unicode; }

String Beatmap::get_artist() { return artist; }

String Beatmap::get_artist_unicode() { return artist_unicode; }

String Beatmap::get_creator() { return creator; }

String Beatmap::get_version() { return version; }

String Beatmap::get_source() { return source; }

PoolStringArray Beatmap::get_tags() { return tags; }

String Beatmap::get_beatmap_id() { return beatmap_id; }

String Beatmap::get_beatmap_set_id() { return beatmap_set_id; }

float Beatmap::get_hp_drain_rate() { return hp_drain_rate; }

float Beatmap::get_circle_size() { return circle_size; }

float Beatmap::get_overall_difficulty() { return overall_difficulty; }

float Beatmap::get_approach_rate() { return approach_rate; }

float Beatmap::get_slider_multiplier() { return slider_multiplier; }

float Beatmap::get_slider_tick_rate() { return slider_tick_rate; }

String Beatmap::get_background_filename() { return background_filename; }

vector<BreakPeriod *> Beatmap::get_break_periods() { return break_periods; }

vector<TimingPoint *> Beatmap::get_timing_points() { return timing_points; }

vector<HitObject *> Beatmap::get_hit_objects() { return hit_objects; }

void Beatmap::set_audio_filename(String audio_filename) {
    this->audio_filename = audio_filename;
}

void Beatmap::set_preview_time(int64_t preview_time) {
    this->preview_time = preview_time;
}

void Beatmap::set_beat_divisor(int64_t beat_divisor) {
    this->beat_divisor = beat_divisor;
}

void Beatmap::set_timeline_zoom(float timeline_zoom) {
    this->timeline_zoom = timeline_zoom;
    emit_signal("timeline_zoom_updated", timeline_zoom);
}

void Beatmap::set_title(String title) {
    this->title = title;
    emit_signal("title_updated", title);
}

void Beatmap::set_title_unicode(String title_unicode) {
    this->title_unicode = title_unicode;
    emit_signal("title_unicode_updated", title_unicode);
}

void Beatmap::set_artist(String artist) {
    this->artist = artist;
    emit_signal("artist_updated", artist);
}

void Beatmap::set_artist_unicode(String artist_unicode) {
    this->artist_unicode = artist_unicode;
    emit_signal("artist_unicode_updated", artist_unicode);
}

void Beatmap::set_creator(String creator) {
    this->creator = creator;
    emit_signal("creator_updated", creator);
}

void Beatmap::set_version(String version) {
    this->version = version;
    emit_signal("version_updated", version);
}

void Beatmap::set_source(String source) {
    this->source = source;
    emit_signal("source_updated", source);
}

void Beatmap::set_tags(PoolStringArray tags) {
    this->tags = tags;
    emit_signal("tags_updated", tags);
}

void Beatmap::set_beatmap_id(String beatmap_id) {
    this->beatmap_id = beatmap_id;
}

void Beatmap::set_beatmap_set_id(String beatmap_set_id) {
    this->beatmap_set_id = beatmap_set_id;
}

void Beatmap::set_hp_drain_rate(float hp_drain_rate) {
    this->hp_drain_rate = hp_drain_rate;
    emit_signal("hp_drain_rate_updated", hp_drain_rate);
}

void Beatmap::set_circle_size(float circle_size) {
    this->circle_size = circle_size;
    emit_signal("circle_size_updated", circle_size);
}

void Beatmap::set_overall_difficulty(float overall_difficulty) {
    this->overall_difficulty = overall_difficulty;
    emit_signal("overall_difficulty_updated", overall_difficulty);
}

void Beatmap::set_approach_rate(float approach_rate) {
    this->approach_rate = approach_rate;
    emit_signal("approach_rate_updated", approach_rate);
}

void Beatmap::set_slider_multiplier(float slider_multiplier) {
    this->slider_multiplier = slider_multiplier;
}

void Beatmap::set_slider_tick_rate(float slider_tick_rate) {
    this->slider_tick_rate = slider_tick_rate;
}

void Beatmap::set_background_filename(String background_filename) {
    this->background_filename = background_filename;
    emit_signal("background_filename_updated", background_filename);
}

void Beatmap::set_break_periods(vector<BreakPeriod *> break_periods) {
    this->break_periods = break_periods;
}

void Beatmap::set_timing_points(vector<TimingPoint *> timing_points) {
    this->timing_points = timing_points;
}

void Beatmap::set_hit_objects(vector<HitObject *> hit_objects) {
    this->hit_objects = hit_objects;
}

bool Beatmap::has_query(String query) {
    if (query.empty()) return true;

    query = query.to_lower();

    if (title.to_lower().find(query) >= 0) return true;
    if (title_unicode.to_lower().find(query) >= 0) return true;
    if (artist.to_lower().find(query) >= 0) return true;
    if (artist_unicode.to_lower().find(query) >= 0) return true;
    if (creator.to_lower().find(query) >= 0) return true;
    if (version.to_lower().find(query) >= 0) return true;
    if (source.to_lower().find(query) >= 0) return true;

    for (int i = 0; i < tags.size(); i++) {
        if (tags[i].to_lower().find(query) >= 0) return true;
    }

    return false;
}

void Beatmap::copy(Beatmap *beatmap) {
    audio_filename = beatmap->audio_filename;
    preview_time = beatmap->preview_time;
    beat_divisor = beatmap->beat_divisor;
    timeline_zoom = beatmap->timeline_zoom;
    title = beatmap->title;
    title_unicode = beatmap->title_unicode;
    artist = beatmap->artist;
    artist_unicode = beatmap->artist_unicode;
    creator = beatmap->creator;
    version = beatmap->version;
    source = beatmap->source;

    tags = PoolStringArray();
    tags.append_array(beatmap->tags);

    beatmap_id = beatmap->beatmap_id;
    beatmap_set_id = beatmap->beatmap_set_id;

    hp_drain_rate = beatmap->hp_drain_rate;
    circle_size = beatmap->circle_size;
    overall_difficulty = beatmap->overall_difficulty;
    approach_rate = beatmap->approach_rate;
    slider_multiplier = beatmap->slider_multiplier;
    slider_tick_rate = beatmap->slider_tick_rate;

    background_filename = beatmap->background_filename;

    for (auto e : break_periods) e->queue_free();
    break_periods.clear();
    for (auto e : beatmap->break_periods) {
        auto bp = BreakPeriod::_new();
        bp->copy(e);
        break_periods.push_back(bp);
    }

    for (auto e : timing_points) e->queue_free();
    timing_points.clear();
    for (auto e : beatmap->timing_points) {
        auto tp = TimingPoint::_new();
        tp->copy(e);
        timing_points.push_back(tp);
    }

    for (auto e : hit_objects) e->queue_free();
    hit_objects.clear();
    for (auto e : beatmap->hit_objects) {
        auto ho = HitObject::_new();
        ho->copy(e);
        hit_objects.push_back(ho);
    }
}

String Beatmap::get_dir_path(Node *node, Beatmap *beatmap) {
    return Game::get_singleton(node)->get_songs_dir_path() + "/" +
           beatmap->get_beatmap_set_id();
}

String Beatmap::get_file_path(Node *node, Beatmap *beatmap) {
    return get_dir_path(node, beatmap) + "/" + beatmap->get_beatmap_id() +
           MapManager::get_singleton(node)->get_map_extension();
}

String Beatmap::get_background_file_path(Node *node, Beatmap *beatmap) {
    String bg_filename = beatmap->get_background_filename();
    if (bg_filename.empty()) {
        return Game::get_singleton(node)->get_default_background_path();
    } else {
        return get_dir_path(node, beatmap) + "/" + bg_filename;
    }
}

TimingPoint *Beatmap::get_control_point_for_time(int64_t ms) {
    TimingPoint *ret = nullptr;

    for (auto tp : timing_points) {
        if (tp->is_uninherited()) {
            if (ret == nullptr) {
                ret = tp;
            } else if (tp->get_time() <= ms) {
                ret = tp;
            }
        }
    }

    return ret;
}

vector<HitObject *> Beatmap::find_hit_objects(int64_t start_time,
                                              int64_t end_time) {
    vector<HitObject *> ret;

    for (auto ho : hit_objects) {
        auto time = ho->get_start_time();
        if (start_time <= time && time <= end_time) {
            ret.push_back(ho);
        }
    }

    return ret;
}