#include "./song_setup_body.h"

#include "object/enum/song_metadata_type.h"
#include "singleton/map_manager.h"

void SongSetupBody::_register_methods() {
    register_method("_ready", &SongSetupBody::_ready);
    register_method("on_song_metadata_update",
                    &SongSetupBody::on_song_metadata_update);

    register_signal<SongSetupBody>("song_metadata_update", "index",
                                   GODOT_VARIANT_TYPE_INT, "value",
                                   GODOT_VARIANT_TYPE_STRING);
}

void SongSetupBody::_init() {}

void SongSetupBody::_ready() { init_song_metadata(); }

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