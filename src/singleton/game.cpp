#include "./game.h"

#include <Directory.hpp>
#include <File.hpp>
#include <Input.hpp>
#include <OS.hpp>
#include <ProjectSettings.hpp>

#include "singleton/map_manager.h"

Game* Game::get_singleton(Node* node) {
    return node->get_node<Game>("/root/Game");
}

void Game::_register_methods() { register_method("_ready", &Game::_ready); }

void Game::_init() {
    export_extension = ".osz";
    songs_dir_path = "user://songs";
    export_dir_path = "user://exports";
    default_background_path = "res://assets/images/default-background.jpg";
}

void Game::_ready() {
    Godot::print("game ready");
    set_confine_mouse(true);
    init_directory(songs_dir_path);
    init_directory(export_dir_path);
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

void Game::init_directory(String path) {
    auto dir = Directory::_new();

    if (!dir->dir_exists(path)) {
        dev_assert(dir->make_dir(path) == Error::OK);
        Godot::print("created directory: " + path);
    }
}

String Game::get_songs_dir_path() { return songs_dir_path; }

String Game::get_default_background_path() { return default_background_path; }

void Game::open_directory(String path, bool is_global) {
    String global_path = path;

    if (!is_global) {
        global_path = ProjectSettings::get_singleton()->globalize_path(path);
    }

    auto code = OS::get_singleton()->shell_open(global_path);
    Godot::print("opening dir(" + String::num_int64(+code) +
                 "): " + global_path);
}

void Game::export_beatmap(Beatmap* beatmap) {
    String path = Beatmap::get_dir_path(this, beatmap);
    String input_path = ProjectSettings::get_singleton()->globalize_path(path);

    String output_path = ProjectSettings::get_singleton()->globalize_path(
        export_dir_path + "/" + beatmap->get_beatmap_set_id() +
        export_extension);

    auto args = PoolStringArray(Array::make(
        "a", "-tzip", "\"" + output_path + "\"", "\"" + input_path + "\""));
    int64_t exit_code = OS::get_singleton()->execute("7z", args);
    Godot::print("export beatmap exit code: " + String::num_int64(exit_code));
}

String Game::get_export_dir_path() { return export_dir_path; }