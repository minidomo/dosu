#include "./map_manager.h"

#include <Directory.hpp>
#include <File.hpp>

#include "./game.h"

MapManager* MapManager::get_singleton(Node* node) {
    return node->get_node<MapManager>("/root/MapManager");
}

void MapManager::_register_methods() {
    register_method("_ready", &MapManager::_ready);
}

void MapManager::_init() {
    audio_extensions.append(".mp3");
    map_extension = ".osu";
}

void MapManager::_ready() { Godot::print("map manager ready"); }

void MapManager::create_map(String set_id) {
    auto dir = Directory::_new();

    String set_path =
        Game::get_singleton(this)->get_songs_dir_path() + "/" + set_id;

    String map_id = create_unique_map_id(set_id);

    String map_path = set_path + "/" + map_id + ".osu";

    File* file = File::_new();
    dev_assert(file->open(map_path, File::WRITE) == Error::OK);

    // TODO create beatmap default info
    file->store_line(map_path);

    file->close();
}

void MapManager::create_set(String audio_path) {
    auto dir = Directory::_new();

    String set_id = create_unique_set_id();
    String set_path =
        Game::get_singleton(this)->get_songs_dir_path() + "/" + set_id;

    dev_assert(dir->make_dir(set_path) == Error::OK);

    String new_audio_path =
        set_path + "/audio" + extract_audio_extension(audio_path);
    dev_assert(dir->copy(audio_path, new_audio_path) == Error::OK);

    create_map(set_id);
}

String MapManager::create_unique_set_id() {
    auto dir = Directory::_new();

    dev_assert(dir->open(Game::get_singleton(this)->get_songs_dir_path()) ==
               Error::OK);

    Dictionary dict;

    dir->list_dir_begin();
    for (String id = dir->get_next(); id != ""; id = dir->get_next()) {
        if (dir->current_is_dir()) {
            dict[id] = true;
        }
    }

    int64_t id = 0;
    while (dict.has(String::num_int64(id))) {
        id++;
    }

    return String::num_int64(id);
}

String MapManager::create_unique_map_id(String set_id) {
    auto dir = Directory::_new();

    String set_path =
        Game::get_singleton(this)->get_songs_dir_path() + "/" + set_id;

    dev_assert(dir->open(set_path) == Error::OK);

    Dictionary dict;

    dir->list_dir_begin();
    for (String name = dir->get_next(); name != ""; name = dir->get_next()) {
        if (!dir->current_is_dir() && name.ends_with(map_extension)) {
            String id = name.substr(0, name.length() - map_extension.length());
            dict[name] = true;
        }
    }

    int64_t id = 0;
    while (dict.has(id)) {
        id++;
    }

    return String::num_int64(id);
}

bool MapManager::is_valid_audio_extension(String file_path) {
    for (int i = 0; i < audio_extensions.size(); i++) {
        String audio_ext = audio_extensions[i];
        if (file_path.ends_with(audio_ext)) {
            return true;
        }
    }

    return false;
}

String MapManager::extract_audio_extension(String file_path) {
    for (int i = 0; i < audio_extensions.size(); i++) {
        String audio_ext = audio_extensions[i];
        if (file_path.ends_with(audio_ext)) {
            return audio_ext;
        }
    }

    return "";
}
