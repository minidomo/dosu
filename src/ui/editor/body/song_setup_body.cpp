#include "./song_setup_body.h"

#include "object/enum/song_difficulty_type.h"
#include "object/enum/song_metadata_type.h"
#include "singleton/map_manager.h"

void SongSetupBody::_register_methods() {
    register_method("_ready", &SongSetupBody::_ready);
    register_method("on_song_metadata_update",
                    &SongSetupBody::on_song_metadata_update);
    register_method("on_song_difficulty_update",
                    &SongSetupBody::on_song_difficulty_update);

    register_signal<SongSetupBody>("song_metadata_update", "index",
                                   GODOT_VARIANT_TYPE_INT, "value",
                                   GODOT_VARIANT_TYPE_STRING);
    register_signal<SongSetupBody>("song_difficulty_update", "index",
                                   GODOT_VARIANT_TYPE_INT, "value",
                                   GODOT_VARIANT_TYPE_STRING);
}

void SongSetupBody::_init() {}

void SongSetupBody::_ready() {
    init_song_metadata();
    init_song_difficulty();
}

void SongSetupBody::init_song_metadata() {
    Node *container =
        get_node("ColorRect/SongMetadata/MarginContainer/VBoxContainer");
    Array children = container->get_children();

    for (int i = 0; i < children.size(); i++) {
        auto row = Object::cast_to<EditableRow>(children[i]);
        song_metadata.push_back(row);

        row->get_line_edit()->connect(
            "text_changed", this, "on_song_metadata_update", Array::make(i));
    }

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();

    song_metadata[+SongMetadataType::Artist]->set_value(beatmap->get_artist());
    song_metadata[+SongMetadataType::ArtistUnicode]->set_value(
        beatmap->get_artist_unicode());
    song_metadata[+SongMetadataType::Title]->set_value(beatmap->get_title());
    song_metadata[+SongMetadataType::TitleUnicode]->set_value(
        beatmap->get_title_unicode());
    song_metadata[+SongMetadataType::Creator]->set_value(
        beatmap->get_creator());
    song_metadata[+SongMetadataType::Difficulty]->set_value(
        beatmap->get_version());
    song_metadata[+SongMetadataType::Source]->set_value(beatmap->get_source());
    song_metadata[+SongMetadataType::Tags]->set_value(
        beatmap->to_string_tags());
}

void SongSetupBody::on_song_metadata_update(String new_text, int index) {
    emit_signal("song_metadata_update", index, new_text);
}

void SongSetupBody::init_song_difficulty() {
    Node *container =
        get_node("ColorRect/DifficultyMetadata/MarginContainer/VBoxContainer");
    Array children = container->get_children();

    for (int i = 0; i < children.size(); i++) {
        auto row = Object::cast_to<SlidableRow>(children[i]);
        song_difficulty.push_back(row);

        row->get_slider()->connect("value_changed", this,
                                   "on_song_difficulty_update", Array::make(i));
    }

    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();

    song_difficulty[+SongDifficultyType::HpDrainRate]->set_value(
        beatmap->get_hp_drain_rate());
    song_difficulty[+SongDifficultyType::CircleSize]->set_value(
        beatmap->get_circle_size());
    song_difficulty[+SongDifficultyType::ApproachRate]->set_value(
        beatmap->get_approach_rate());
    song_difficulty[+SongDifficultyType::OverallDifficulty]->set_value(
        beatmap->get_overall_difficulty());
}

void SongSetupBody::on_song_difficulty_update(float value, int index) {
    emit_signal("song_difficulty_update", index, value);
}