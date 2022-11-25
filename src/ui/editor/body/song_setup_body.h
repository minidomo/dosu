#ifndef dosu_file_song_setup_body
#define dosu_file_song_setup_body

#include <Control.hpp>
#include <vector>

#include "common/common.h"
#include "ui/editor/editable_row.h"
#include "ui/editor/slidable_row.h"

class SongSetupBody : public Control {
    GODOT_CLASS(SongSetupBody, Control);

   private:
    EditableRow *artist_row;
    EditableRow *artist_unicode_row;
    EditableRow *title_row;
    EditableRow *title_unicode_row;
    EditableRow *creator_row;
    EditableRow *difficulty_row;
    EditableRow *source_row;
    EditableRow *tags_row;

    SlidableRow *hp_drain_rate_row;
    SlidableRow *circle_size_row;
    SlidableRow *approach_rate_row;
    SlidableRow *overall_difficulty_row;

   public:
    static void _register_methods();
    void _init();

    void _ready();

    void init_song_metadata();
    void init_song_difficulty();

    void on_song_difficulty_update(float value, int index);

    void on_artist_row_updated(String value);
    void on_artist_unicode_row_updated(String value);
    void on_title_row_updated(String value);
    void on_title_unicode_row_updated(String value);
    void on_creator_row_updated(String value);
    void on_difficulty_row_updated(String value);
    void on_source_row_updated(String value);
    void on_tags_row_updated(String value);

    void on_hp_drain_rate_row_updated(float value);
    void on_circle_size_row_updated(float value);
    void on_approach_rate_row_updated(float value);
    void on_overall_difficulty_row_updated(float value);
};

#endif