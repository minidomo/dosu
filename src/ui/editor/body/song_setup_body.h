#ifndef dosu_file_song_setup_body
#define dosu_file_song_setup_body

#include <Control.hpp>
#include <vector>

#include "common/common.h"
#include "ui/editor/editable_row.h"

class SongSetupBody : public Control {
    GODOT_CLASS(SongSetupBody, Control);

   private:
    vector<EditableRow *> song_metadata;

   public:
    static void _register_methods();
    void _init();

    void _ready();

    void init_song_metadata();

    void on_song_metadata_update(String new_text, int index);
};

#endif