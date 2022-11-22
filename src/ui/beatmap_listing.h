#ifndef dosu_file_beatmap_listing
#define dosu_file_beatmap_listing

#include <ColorRect.hpp>
#include <Control.hpp>
#include <Label.hpp>

#include "common/common.h"

class BeatmapListing : public Control {
    GODOT_CLASS(BeatmapListing, Control);

   private:
    String title;
    String artist;
    String mapper;
    String difficulty_name;

    ColorRect *outer;
    ColorRect *inner;

    Label *title_label;
    Label *artist_mapper_label;
    Label *version_label;

   public:
    static void _register_methods();
    void _init();

    void _ready();

    void color_scheme_select();
    void color_scheme_unselect();

    void set_title(String title);
    void set_artist(String artist);
    void set_mapper(String mapper);
    void set_difficulty_name(String difficulty_name);

    String get_title();
    String get_artist();
    String get_mapper();
    String get_difficulty_name();
};

#endif