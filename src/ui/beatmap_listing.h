#ifndef dosu_file_beatmap_listing
#define dosu_file_beatmap_listing

#include <ColorRect.hpp>
#include <Control.hpp>
#include <InputEvent.hpp>
#include <Label.hpp>
#include <Ref.hpp>

#include "common/common.h"

class BeatmapListing : public Control {
    GODOT_CLASS(BeatmapListing, Control);

   private:
    int64_t global_index;
    int64_t local_index;

    String title;
    String artist;
    String mapper;
    String difficulty_name;
    bool selected;

    ColorRect *outer;
    ColorRect *inner;

    Label *title_label;
    Label *artist_mapper_label;
    Label *version_label;

   public:
    static void _register_methods();
    void _init();

    void _ready();
    void _gui_input(Ref<InputEvent> event);

    void color_scheme_select();
    void color_scheme_unselect();

    void set_title(String title);
    void set_artist(String artist);
    void set_mapper(String mapper);
    void set_difficulty_name(String difficulty_name);
    void set_selected(bool selected);
    void set_global_index(int64_t global_index);
    void set_local_index(int64_t local_index);

    String get_title();
    String get_artist();
    String get_mapper();
    String get_difficulty_name();
    bool is_selected();
    int64_t get_global_index();
    int64_t get_local_index();

    void update_view();
};

#endif