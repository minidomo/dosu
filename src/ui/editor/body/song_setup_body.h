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
    vector<EditableRow *> song_metadata;
    vector<SlidableRow *> song_difficulty;

   public:
    static void _register_methods();
    void _init();

    void _ready();

    void init_song_metadata();
    void init_song_difficulty();

    void on_song_metadata_update(String new_text, int index);
    void on_song_difficulty_update(float value, int index);
};

#endif