#include "./beatmap_listing.h"

void BeatmapListing::_register_methods() {
    register_method("_ready", &BeatmapListing::_ready);
}

void BeatmapListing::_init() {}

void BeatmapListing::_ready() {
    outer = get_node<ColorRect>("Background/Outer");
    inner = get_node<ColorRect>("Background/MarginContainer/Inner");
    title_label = get_node<Label>("TitleLabel");
    artist_mapper_label = get_node<Label>("ArtistMapperLabel");
    version_label = get_node<Label>("VersionLabel");
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

void BeatmapListing::set_title(String title) {
    this->title = title;
    title_label->set_text(title);
}

void BeatmapListing::set_artist(String artist) {
    this->artist = artist;
    artist_mapper_label->set_text(artist + " // " + mapper);
}

void BeatmapListing::set_mapper(String mapper) {
    this->mapper = mapper;
    artist_mapper_label->set_text(artist + " // " + mapper);
}

void BeatmapListing::set_difficulty_name(String difficulty_name) {
    this->difficulty_name = difficulty_name;
    version_label->set_text(difficulty_name);
}

String BeatmapListing::get_title() { return title; }

String BeatmapListing::get_artist() { return artist; }

String BeatmapListing::get_mapper() { return mapper; }

String BeatmapListing::get_difficulty_name() { return difficulty_name; }
