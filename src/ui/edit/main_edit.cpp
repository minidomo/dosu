#include "./main_edit.h"

#include <SceneTree.hpp>

#include "../../singleton/game.h"
#include "../../singleton/map_manager.h"
#include "../../singleton/scene_manager.h"

void MainEdit::_register_methods() {
    register_method("_ready", &MainEdit::_ready);

    register_method("on_select_button_pressed",
                    &MainEdit::on_select_button_pressed);
    register_method("on_back_button_pressed",
                    &MainEdit::on_back_button_pressed);

    register_method("on_files_dropped", &MainEdit::on_files_dropped);

    register_property<MainEdit, NodePath>(
        "select_button_path", &MainEdit::select_button_path, NodePath());
    register_property<MainEdit, NodePath>(
        "back_button_path", &MainEdit::back_button_path, NodePath());
}

void MainEdit::_init() {
    select_button_path = NodePath();
    back_button_path = NodePath();
}

void MainEdit::_ready() {
    dev_assert(!select_button_path.is_empty());
    dev_assert(!back_button_path.is_empty());

    select_button = get_node<Button>(select_button_path);
    back_button = get_node<Button>(back_button_path);

    Game::get_singleton(this)->set_borderless(true);
    Game::get_singleton(this)->set_confine_mouse(false);

    select_button->connect("pressed", this, "on_select_button_pressed");
    back_button->connect("pressed", this, "on_back_button_pressed");

    get_tree()->connect("files_dropped", this, "on_files_dropped");
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
    } else {
        // TODO display error message to user?
    }
}
