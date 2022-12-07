#include "./scene_manager.h"

#include <SceneTree.hpp>

#include "common/util.h"

SceneManager* SceneManager::get_singleton(Node* node) {
    return node->get_node<SceneManager>("/root/SceneManager");
}

void SceneManager::_register_methods() {
    register_method("_ready", &SceneManager::_ready);
}

void SceneManager::_init() {}

void SceneManager::_ready() { Godot::print("scene manager ready"); }

void SceneManager::exit() { get_tree()->quit(); }

void SceneManager::to_edit_scene() {
    font_ids.clear();
    get_tree()->change_scene("res://scenes/edit/MainEdit.tscn");
}

void SceneManager::to_play_scene() { Godot::print("TODO play scene"); }

void SceneManager::to_main_menu_scene() {
    font_ids.clear();
    get_tree()->change_scene("res://scenes/MainMenu.tscn");
}

void SceneManager::to_editor_scene() {
    font_ids.clear();
    get_tree()->change_scene("res://scenes/editor/MainEditor.tscn");
}

void SceneManager::recursive_scale_font(Control* node) {
    Util::recursive_scale_font(node, font_ids);
}