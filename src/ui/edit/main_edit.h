#ifndef dosu_file_main_edit
#define dosu_file_main_edit

#include <Button.hpp>
#include <Control.hpp>
#include <Label.hpp>
#include <LineEdit.hpp>
#include <VBoxContainer.hpp>

#include "common/common.h"
#include "object/beatmap.h"
#include "ui/background.h"
#include "ui/beatmap_listing.h"

class MainEdit : public Control {
    GODOT_CLASS(MainEdit, Control);

   private:
    Background *background;
    LineEdit *search_bar;
    Button *select_button;
    Button *back_button;
    VBoxContainer *map_container;
    Label *matches_label;
    Button *refresh_button;

    void update_beatmaps(String filter);
    BeatmapListing *find_beatmap_listing(int64_t global_index);

   public:
    static void _register_methods();
    void _init();

    void _ready();

    void on_select_button_pressed();
    void on_back_button_pressed();
    void on_files_dropped(PoolStringArray files, int screen);
    void on_text_changed(String new_text);
    void on_beatmap_index_changed(int64_t old_index, int64_t new_index);
    void on_refresh_button_pressed();
};

#endif