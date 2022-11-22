#include "./main_edit.h"

#include <PackedScene.hpp>
#include <Ref.hpp>
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>

#include "object/beatmap.h"
#include "singleton/game.h"
#include "singleton/map_manager.h"
#include "singleton/scene_manager.h"
#include "ui/beatmap_listing.h"

void MainEdit::_register_methods() {
    register_method("_ready", &MainEdit::_ready);

    register_method("on_select_button_pressed",
                    &MainEdit::on_select_button_pressed);
    register_method("on_back_button_pressed",
                    &MainEdit::on_back_button_pressed);

    register_method("on_files_dropped", &MainEdit::on_files_dropped);
}

void MainEdit::_init() {}

void MainEdit::_ready() {
    select_button = get_node<Button>("BottomBar/MarginContainer2/SelectButton");
    back_button = get_node<Button>("BottomBar/MarginContainer/BackButton");

    Game::get_singleton(this)->set_borderless(true);
    Game::get_singleton(this)->set_confine_mouse(false);

    select_button->connect("pressed", this, "on_select_button_pressed");
    back_button->connect("pressed", this, "on_back_button_pressed");

    get_tree()->connect("files_dropped", this, "on_files_dropped");

    map_container =
        get_node<VBoxContainer>("RightBody/ScrollContainer/VBoxContainer");
    update_beatmaps();
}

void MainEdit::on_select_button_pressed() {
    Godot::print("enter editor with the selected beat map");
}

void MainEdit::on_back_button_pressed() {
    SceneManager::get_singleton(this)->to_main_menu_scene();
}

void MainEdit::on_files_dropped(PoolStringArray files, int screen) {
    String path = files[0];

    if (MapManager::get_singleton(this)->is_valid_audio_extension(path)) {
        MapManager::get_singleton(this)->create_set(path);
        SceneManager::get_singleton(this)->to_editor_scene();
    } else {
        // TODO display error message to user?
    }
}

void MainEdit::update_beatmaps() {
    auto beatmaps = MapManager::get_singleton(this)->get_all_beatmaps();

    for (int i = 0; i < map_container->get_child_count(); i++) {
        Node *child = map_container->get_child(i);
        map_container->remove_child(child);
        child->queue_free();
    }

    Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(
        "res://scenes/BeatmapListing.tscn");

    for (auto bm : beatmaps) {
        BeatmapListing *listing =
            Object::cast_to<BeatmapListing>(scene->instance());

        listing->set_title(bm.get_title());
        listing->set_artist(bm.get_artist());
        listing->set_mapper(bm.get_creator());
        listing->set_difficulty_name(bm.get_version());

        map_container->add_child(listing);
    }
}
