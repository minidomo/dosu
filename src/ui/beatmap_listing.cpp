#include "./beatmap_listing.h"

#include "singleton/map_manager.h"

void BeatmapListing::_register_methods() {
    register_method("_ready", &BeatmapListing::_ready);
    register_method("_gui_input", &BeatmapListing::_gui_input);
}

void BeatmapListing::_init() {}

void BeatmapListing::_ready() {
    outer = get_node<ColorRect>("Background/Outer");
    inner = get_node<ColorRect>("Background/MarginContainer/Inner");
    title_label = get_node<Label>("TitleLabel");
    artist_mapper_label = get_node<Label>("ArtistMapperLabel");
    version_label = get_node<Label>("VersionLabel");

    update_view();
}

void BeatmapListing::_gui_input(Ref<InputEvent> event) {
    if (event->is_action_pressed("primary_click")) {
        MapManager::get_singleton(this)->set_selected_beatmap_index(
            global_index);
    }
}

void BeatmapListing::color_scheme_select() {
    outer->set_frame_color(Color::hex(0x0e1124dc));
    inner->set_frame_color(Color::hex(0x2222350a));
    title_label->add_color_override("font_color", Color::hex(0x9ca9d4ff));
    artist_mapper_label->add_color_override("font_color",
                                            Color::hex(0x9ca9d4ff));
    version_label->add_color_override("font_color", Color::hex(0x9ca9d4ff));
}

void BeatmapListing::color_scheme_unselect() {
    outer->set_frame_color(Color::hex(0x010D28dc));
    inner->set_frame_color(Color::hex(0xffffff05));
    title_label->add_color_override("font_color", Color::hex(0x984c5846));
    artist_mapper_label->add_color_override("font_color",
                                            Color::hex(0x984c5846));
    version_label->add_color_override("font_color", Color::hex(0x984c58ff));
}

void BeatmapListing::set_title(String title) { this->title = title; }

void BeatmapListing::set_artist(String artist) { this->artist = artist; }

void BeatmapListing::set_mapper(String mapper) { this->mapper = mapper; }

void BeatmapListing::set_difficulty_name(String difficulty_name) {
    this->difficulty_name = difficulty_name;
}

void BeatmapListing::set_selected(bool selected) { this->selected = selected; }

void BeatmapListing::set_global_index(int64_t global_index) {
    this->global_index = global_index;
}

void BeatmapListing::set_local_index(int64_t local_index) {
    this->local_index = local_index;
}

String BeatmapListing::get_title() { return title; }

String BeatmapListing::get_artist() { return artist; }

String BeatmapListing::get_mapper() { return mapper; }

String BeatmapListing::get_difficulty_name() { return difficulty_name; }

bool BeatmapListing::is_selected() { return selected; }

int64_t BeatmapListing::get_global_index() { return global_index; }

int64_t BeatmapListing::get_local_index() { return local_index; }

void BeatmapListing::update_view() {
    title_label->set_text(title);
    artist_mapper_label->set_text(artist + " // " + mapper);
    version_label->set_text(difficulty_name);

    if (selected) {
        color_scheme_select();
    } else {
        color_scheme_unselect();
    }
}
