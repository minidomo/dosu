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
};

#endif