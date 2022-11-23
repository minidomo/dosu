#include "./main_edit.h"

#include <PackedScene.hpp>
#include <Ref.hpp>
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>

#include "common/util.h"
#include "singleton/game.h"
#include "singleton/map_manager.h"
#include "singleton/scene_manager.h"

void MainEdit::_register_methods() {
    register_method("_ready", &MainEdit::_ready);

    register_method("on_select_button_pressed",
                    &MainEdit::on_select_button_pressed);
    register_method("on_back_button_pressed",
                    &MainEdit::on_back_button_pressed);

    register_method("on_files_dropped", &MainEdit::on_files_dropped);
    register_method("on_text_changed", &MainEdit::on_text_changed);
    register_method("on_beatmap_index_changed",
                    &MainEdit::on_beatmap_index_changed);
}

void MainEdit::_init() {}

void MainEdit::_ready() {
    Game::get_singleton(this)->set_borderless(true);
    Game::get_singleton(this)->set_confine_mouse(false);

    select_button = get_node<Button>("BottomBar/MarginContainer2/SelectButton");
    back_button = get_node<Button>("BottomBar/MarginContainer/BackButton");

    select_button->connect("pressed", this, "on_select_button_pressed");
    back_button->connect("pressed", this, "on_back_button_pressed");

    get_tree()->connect("files_dropped", this, "on_files_dropped");

    matches_label = get_node<Label>("RightBody/SearchBody/MatchesLabel");
    background = get_node<Background>("Background");

    map_container =
        get_node<VBoxContainer>("RightBody/ScrollContainer/VBoxContainer");
    update_beatmaps("");

    search_bar = get_node<LineEdit>("RightBody/SearchBody/SearchBar");
    search_bar->connect("text_changed", this, "on_text_changed");

    MapManager::get_singleton(this)->connect("beatmap_index_changed", this,
                                             "on_beatmap_index_changed");
}

void MainEdit::on_select_button_pressed() {
    MapManager::get_singleton(this)->refresh_editor_beatmap();
    SceneManager::get_singleton(this)->to_editor_scene();
}

void MainEdit::on_back_button_pressed() {
    SceneManager::get_singleton(this)->to_main_menu_scene();
}

void MainEdit::on_files_dropped(PoolStringArray files, int screen) {
    String path = files[0];

    auto map_manager = MapManager::get_singleton(this);

    if (map_manager->is_valid_audio_extension(path)) {
        map_manager->create_set(path);
        map_manager->set_selected_beatmap_index(
            map_manager->get_all_beatmaps().size() - 1);
        MapManager::get_singleton(this)->refresh_editor_beatmap();
        SceneManager::get_singleton(this)->to_editor_scene();
    } else {
        // TODO display error message to user?
    }
}

void MainEdit::update_beatmaps(String filter) {
    auto beatmaps = MapManager::get_singleton(this)->get_all_beatmaps();

    Util::delete_children(map_container);

    Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(
        "res://scenes/BeatmapListing.tscn");

    int64_t local_index = 0;
    for (int i = 0; i < beatmaps.size(); i++) {
        auto bm = beatmaps[i];

        if (bm.has_query(filter)) {
            BeatmapListing *listing =
                Object::cast_to<BeatmapListing>(scene->instance());

            listing->set_title(bm.get_title());
            listing->set_artist(bm.get_artist());
            listing->set_mapper(bm.get_creator());
            listing->set_difficulty_name(bm.get_version());
            listing->set_selected(
                i ==
                MapManager::get_singleton(this)->get_selected_beatmap_index());
            listing->set_global_index(i);
            listing->set_local_index(local_index);

            map_container->add_child(listing);
            local_index++;

            if (listing->is_selected()) {
                Background::update_background(background, &bm);
            }
        }
    }

    if (filter.empty()) {
        matches_label->set_text("");
    } else {
        matches_label->set_text(String::num_int64(local_index) +
                                " matches found!");
    }
}

void MainEdit::on_text_changed(String new_text) { update_beatmaps(new_text); }

void MainEdit::on_beatmap_index_changed(int64_t old_index, int64_t new_index) {
    auto old_listing = find_beatmap_listing(old_index);
    auto new_listing = find_beatmap_listing(new_index);

    if (old_listing) {
        old_listing->set_selected(false);
        old_listing->update_view();
    }

    if (new_listing) {
        new_listing->set_selected(true);
        new_listing->update_view();

        auto beatmaps = MapManager::get_singleton(this)->get_all_beatmaps();
        Background::update_background(background, &beatmaps[new_index]);
    }
}

BeatmapListing *MainEdit::find_beatmap_listing(int64_t global_index) {
    Array children = map_container->get_children();
    for (int i = 0; i < children.size(); i++) {
        auto listing = Object::cast_to<BeatmapListing>(children[i]);
        if (listing->get_global_index() == global_index) {
            return listing;
        }
    }

    return nullptr;
}
