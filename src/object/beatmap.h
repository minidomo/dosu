#ifndef DOSU_BEATMAP_H
#define DOSU_BEATMAP_H

#include <File.hpp>
#include <vector>

#include "../common.h"
#include "./break_period.h"
#include "./hit_object.h"
#include "./timing_point.h"

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

   public:
    Beatmap();
    ~Beatmap();

    void init();
    void parse_contents(File *file);
    void write_contents(File *file);

    String to_string_tags();
    String to_string_background();
};

#endif