#include "./song_setup_body.h"

#include "singleton/map_manager.h"

void SongSetupBody::_register_methods() {
    dev_register_method(SongSetupBody, _ready);

    dev_register_method(SongSetupBody, on_artist_row_updated);
    dev_register_method(SongSetupBody, on_artist_unicode_row_updated);
    dev_register_method(SongSetupBody, on_title_row_updated);
    dev_register_method(SongSetupBody, on_title_unicode_row_updated);
    dev_register_method(SongSetupBody, on_creator_row_updated);
    dev_register_method(SongSetupBody, on_difficulty_row_updated);
    dev_register_method(SongSetupBody, on_source_row_updated);
    dev_register_method(SongSetupBody, on_tags_row_updated);

    dev_register_method(SongSetupBody, on_hp_drain_rate_row_updated);
    dev_register_method(SongSetupBody, on_circle_size_row_updated);
    dev_register_method(SongSetupBody, on_approach_rate_row_updated);
    dev_register_method(SongSetupBody, on_overall_difficulty_row_updated);
}

void SongSetupBody::_init() {}

void SongSetupBody::_ready() {
    init_song_metadata();
    init_song_difficulty();
}

void SongSetupBody::init_song_metadata() {
    Node *container =
        get_node("ColorRect/SongMetadata/MarginContainer/VBoxContainer");

    artist_row = container->get_node<EditableRow>("Artist");
    artist_unicode_row = container->get_node<EditableRow>("ArtistUnicode");
    title_row = container->get_node<EditableRow>("Title");
    title_unicode_row = container->get_node<EditableRow>("TitleUnicode");
    creator_row = container->get_node<EditableRow>("Creator");
    difficulty_row = container->get_node<EditableRow>("Difficulty");
    source_row = container->get_node<EditableRow>("Source");
    tags_row = container->get_node<EditableRow>("Tags");

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    artist_row->set_value(beatmap->get_artist());
    artist_unicode_row->set_value(beatmap->get_artist_unicode());
    title_row->set_value(beatmap->get_title());
    title_unicode_row->set_value(beatmap->get_title_unicode());
    creator_row->set_value(beatmap->get_creator());
    difficulty_row->set_value(beatmap->get_version());
    source_row->set_value(beatmap->get_source());
    tags_row->set_value(beatmap->to_string_tags());

    artist_row->get_line_edit()->connect("text_changed", this,
                                         "on_artist_row_updated");
    artist_unicode_row->get_line_edit()->connect(
        "text_changed", this, "on_artist_unicode_row_updated");
    title_row->get_line_edit()->connect("text_changed", this,
                                        "on_title_row_updated");
    title_unicode_row->get_line_edit()->connect("text_changed", this,
                                                "on_title_unicode_row_updated");
    creator_row->get_line_edit()->connect("text_changed", this,
                                          "on_creator_row_updated");
    difficulty_row->get_line_edit()->connect("text_changed", this,
                                             "on_difficulty_row_updated");
    source_row->get_line_edit()->connect("text_changed", this,
                                         "on_source_row_updated");
    tags_row->get_line_edit()->connect("text_changed", this,
                                       "on_tags_row_updated");
}

void SongSetupBody::init_song_difficulty() {
    Node *container =
        get_node("ColorRect/DifficultyMetadata/MarginContainer/VBoxContainer");

    hp_drain_rate_row = container->get_node<SlidableRow>("HpDrainRate");
    circle_size_row = container->get_node<SlidableRow>("CircleSize");
    approach_rate_row = container->get_node<SlidableRow>("ApproachRate");
    overall_difficulty_row =
        container->get_node<SlidableRow>("OverallDifficulty");

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    hp_drain_rate_row->set_value(beatmap->get_hp_drain_rate());
    circle_size_row->set_value(beatmap->get_circle_size());
    approach_rate_row->set_value(beatmap->get_approach_rate());
    overall_difficulty_row->set_value(beatmap->get_overall_difficulty());

    hp_drain_rate_row->get_slider()->connect("value_changed", this,
                                             "on_hp_drain_rate_row_updated");
    circle_size_row->get_slider()->connect("value_changed", this,
                                           "on_circle_size_row_updated");
    approach_rate_row->get_slider()->connect("value_changed", this,
                                             "on_approach_rate_row_updated");
    overall_difficulty_row->get_slider()->connect(
        "value_changed", this, "on_overall_difficulty_row_updated");
}

void SongSetupBody::on_song_difficulty_update(float value, int index) {
    emit_signal("song_difficulty_update", index, value);
}

void SongSetupBody::on_artist_row_updated(String value) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_artist(value);
}

void SongSetupBody::on_artist_unicode_row_updated(String value) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_artist_unicode(value);
}

void SongSetupBody::on_title_row_updated(String value) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_title(value);
}

void SongSetupBody::on_title_unicode_row_updated(String value) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_title_unicode(value);
}

void SongSetupBody::on_creator_row_updated(String value) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_creator(value);
}

void SongSetupBody::on_difficulty_row_updated(String value) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_version(value);
}

void SongSetupBody::on_source_row_updated(String value) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_source(value);
}

void SongSetupBody::on_tags_row_updated(String value) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_tags(value.split(" "));
}

void SongSetupBody::on_hp_drain_rate_row_updated(float value) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_hp_drain_rate(value);
}

void SongSetupBody::on_circle_size_row_updated(float value) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_circle_size(value);
}

void SongSetupBody::on_approach_rate_row_updated(float value) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_approach_rate(value);
}

void SongSetupBody::on_overall_difficulty_row_updated(float value) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_overall_difficulty(value);
}
