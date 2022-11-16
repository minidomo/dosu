#include "./scene_manager.h"

#include <SceneTree.hpp>

SceneManager* SceneManager::get_singleton(Node* node) {
    return node->get_node<SceneManager>("/root/SceneManager");
}

void SceneManager::_register_methods() {
    register_method("_ready", &SceneManager::_ready);
}

void SceneManager::_init() {}

void SceneManager::_ready() { Godot::print("scene manager ready"); }

void SceneManager::exit() { get_tree()->quit(); }

void SceneManager::to_edit_scene() { Godot::print("edit scene wow"); }

void SceneManager::to_play_scene() { Godot::print("TODO play scene"); }