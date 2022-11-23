#ifndef dosu_file_beatmap
#define dosu_file_beatmap

#include <File.hpp>
#include <vector>

#include "./break_period.h"
#include "./hit_object.h"
#include "./timing_point.h"
#include "common/common.h"

class Beatmap {
   private:
    /* general */
    String audio_filename;
    int64_t preview_time;

    /* editor */
    int64_t beat_divisor;
    float timeline_zoom;

    /* metadata */
    String title;
    String title_unicode;
    String artist;
    String artist_unicode;
    String creator;
    String version;
    String source;
    PoolStringArray tags;
    String beatmap_id;
    String beatmap_set_id;

    /* difficulty */
    float hp_drain_rate;
    float circle_size;
    float overall_difficulty;
    float approach_rate;
    float slider_multiplier;
    float slider_tick_rate;

    /* events */
    String background_filename;
    vector<BreakPeriod> break_periods;

    /* timing points */
    vector<TimingPoint> timing_points;

    /* hit objects */
    vector<HitObject> hit_objects;

    void parse_general_section(PoolStringArray lines);
    void parse_editor_section(PoolStringArray lines);
    void parse_metadata_section(PoolStringArray lines);
    void parse_difficulty_section(PoolStringArray lines);
    void parse_events_section(PoolStringArray lines);
    void parse_timing_points(PoolStringArray lines);
    void parse_hit_objects_section(PoolStringArray lines);

    static String get_right_value(String line, String prefix);
    static PoolStringArray get_lines(File *file);
    static int find_line_index(PoolStringArray lines, String section);

   public:
    Beatmap();
    ~Beatmap();

    void init(String beatmap_set_id, String beatmap_id);
    void parse_contents(File *file);
    void write_contents(File *file);

    String to_string_tags();
    String to_string_background();

    String get_audio_filename();
    int64_t get_preview_time();

    int64_t get_beat_divisor();
    float get_timeline_zoom();

    String get_title();
    String get_title_unicode();
    String get_artist();
    String get_artist_unicode();
    String get_creator();
    String get_version();
    String get_source();
    PoolStringArray get_tags();
    String get_beatmap_id();
    String get_beatmap_set_id();

    float get_hp_drain_rate();
    float get_circle_size();
    float get_overall_difficulty();
    float get_approach_rate();
    float get_slider_multiplier();
    float get_slider_tick_rate();

    String get_background_filename();
    vector<BreakPeriod> get_break_periods();
    vector<TimingPoint> get_timing_points();
    vector<HitObject> get_hit_objects();

    void set_audio_filename(String audio_filename);
    void set_preview_time(int64_t preview_time);

    void set_beat_divisor(int64_t beat_divisor);
    void set_timeline_zoom(float timeline_zoom);

    void set_title(String title);
    void set_title_unicode(String title_unicode);
    void set_artist(String artist);
    void set_artist_unicode(String artist_unicode);
    void set_creator(String creator);
    void set_version(String version);
    void set_source(String source);
    void set_tags(PoolStringArray tags);
    void set_beatmap_id(String beatmap_id);
    void set_beatmap_set_id(String beatmap_set_id);

    void set_hp_drain_rate(float hp_drain_rate);
    void set_circle_size(float circle_size);
    void set_overall_difficulty(float overall_difficulty);
    void set_approach_rate(float approach_rate);
    void set_slider_multiplier(float slider_multiplier);
    void set_slider_tick_rate(float slider_tick_rate);

    void set_background_filename(String background_filename);
    void set_break_periods(vector<BreakPeriod> break_periods);
    void set_timing_points(vector<TimingPoint> timing_points);
    void set_hit_objects(vector<HitObject> hit_objects);

    bool has_query(String query);
    void copy(Beatmap beatmap);
};

#endif