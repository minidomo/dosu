#include "./game.h"

#include <Directory.hpp>
#include <File.hpp>
#include <Input.hpp>
#include <OS.hpp>

#include "singleton/map_manager.h"

Game* Game::get_singleton(Node* node) {
    return node->get_node<Game>("/root/Game");
}

void Game::_register_methods() { register_method("_ready", &Game::_ready); }

void Game::_init() {
    songs_dir_path = "user://songs";
    default_background_path = "res://assets/images/default-background.jpg";
}

void Game::_ready() {
    Godot::print("game ready");
    set_confine_mouse(true);
    init_songs_directory();
    MapManager::get_singleton(this)->load_beatmaps();
    MapManager::get_singleton(this)->randomize_selected_beatmap_index();
}

/**
 * godot doesnt have exclusive fullscreen so fullscreen is always borderless.
 * this function is essentially useless but i'll just keep the logic anyways.
 */
void Game::set_borderless(bool borderless) {
    OS* os = OS::get_singleton();

    if (os->get_borderless_window() != borderless) {
        os->set_borderless_window(borderless);
    }
}

void Game::set_confine_mouse(bool confine) {
    Input* input = Input::get_singleton();

    auto target_mode =
        confine ? Input::MOUSE_MODE_CONFINED : Input::MOUSE_MODE_VISIBLE;

    if (input->get_mouse_mode() != target_mode) {
        input->set_mouse_mode(target_mode);
    }
}

void Game::init_songs_directory() {
    auto dir = Directory::_new();

    if (!dir->dir_exists(songs_dir_path)) {
        dev_assert(dir->make_dir(songs_dir_path) == Error::OK);
        Godot::print("created songs directory: " + songs_dir_path);
    }
}

String Game::get_songs_dir_path() { return songs_dir_path; }

String Game::get_default_background_path() { return default_background_path; }

void Game::open_directory(String path) {
    OS::get_singleton()->shell_open(path);
}