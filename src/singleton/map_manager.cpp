#include "./map_manager.h"

#include <Directory.hpp>
#include <File.hpp>

#include "./game.h"

MapManager* MapManager::get_singleton(Node* node) {
    return node->get_node<MapManager>("/root/MapManager");
}

void MapManager::_register_methods() {
    register_method("_ready", &MapManager::_ready);

    register_signal<MapManager>("beatmap_index_changed", "old_index",
                                GODOT_VARIANT_TYPE_INT, "new_index",
                                GODOT_VARIANT_TYPE_INT);
}

void MapManager::_init() {
    audio_extensions.append(".mp3");
    image_extensions.append(".jpg");
    map_extension = ".osu";
}

void MapManager::_ready() {
    editor_beatmap = Beatmap::_new();
    random = RandomNumberGenerator::_new();
    random->randomize();

    Godot::print("map manager ready");
}

void MapManager::create_map(String set_id, String audio_filename) {
    auto dir = Directory::_new();

    String set_path =
        Game::get_singleton(this)->get_songs_dir_path() + "/" + set_id;

    String map_id = create_unique_map_id(set_id);
    String map_path = set_path + "/" + map_id + map_extension;

    File* file = File::_new();
    dev_assert(file->open(map_path, File::WRITE) == Error::OK);

    Beatmap* beatmap = Beatmap::_new();
    beatmap->initialize(set_id, map_id);
    beatmap->write_contents(file);

    all_beatmaps.push_back(beatmap);

    file->close();
}

void MapManager::create_set(String audio_path) {
    auto dir = Directory::_new();

    String set_id = create_unique_set_id();
    String set_path =
        Game::get_singleton(this)->get_songs_dir_path() + "/" + set_id;

    dev_assert(dir->make_dir(set_path) == Error::OK);

    String audio_filename = "audio" + extract_audio_extension(audio_path);
    String new_audio_path = set_path + "/" + audio_filename;
    dev_assert(dir->copy(audio_path, new_audio_path) == Error::OK);

    create_map(set_id, audio_filename);
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

bool MapManager::is_valid_image_extension(String file_path) {
    for (int i = 0; i < image_extensions.size(); i++) {
        String image_ext = image_extensions[i];
        if (file_path.ends_with(image_ext)) {
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

void MapManager::load_beatmaps() {
    auto dir = Directory::_new();

    dev_assert(dir->open(Game::get_singleton(this)->get_songs_dir_path()) ==
               Error::OK);

    all_beatmaps.clear();

    dir->list_dir_begin();
    for (String set_id = dir->get_next(); set_id != "";
         set_id = dir->get_next()) {
        if (dir->current_is_dir()) {
            String set_path =
                Game::get_singleton(this)->get_songs_dir_path() + "/" + set_id;

            auto set_dir = Directory::_new();

            dev_assert(set_dir->open(set_path) == Error::OK);

            set_dir->list_dir_begin();
            for (String name = set_dir->get_next(); name != "";
                 name = set_dir->get_next()) {
                if (!set_dir->current_is_dir() &&
                    name.ends_with(map_extension)) {
                    String map_path = set_path + "/" + name;

                    File* file = File::_new();
                    dev_assert(file->open(map_path, File::READ) == Error::OK);

                    Beatmap* beatmap = Beatmap::_new();
                    beatmap->parse_contents(file);
                    all_beatmaps.push_back(beatmap);

                    file->close();
                }
            }
        }
    }

    int64_t beatmap_count = all_beatmaps.size();
    Godot::print("loaded " + String::num_int64(beatmap_count) + " beatmaps");
}

vector<Beatmap*> MapManager::get_all_beatmaps() { return all_beatmaps; }

void MapManager::randomize_selected_beatmap_index() {
    set_selected_beatmap_index(random->randi_range(0, all_beatmaps.size() - 1));
}

void MapManager::set_selected_beatmap_index(int64_t selected_beatmap_index) {
    if (selected_beatmap_index != this->selected_beatmap_index) {
        int64_t old_index = this->selected_beatmap_index;
        this->selected_beatmap_index = selected_beatmap_index;

        emit_signal("beatmap_index_changed", old_index, selected_beatmap_index);
    }
}

int64_t MapManager::get_selected_beatmap_index() {
    return selected_beatmap_index;
}

Beatmap* MapManager::get_editor_beatmap() { return editor_beatmap; }

void MapManager::refresh_editor_beatmap() {
    editor_beatmap->copy(all_beatmaps[selected_beatmap_index]);
}

void MapManager::save_editor_beatmap() {
    String set_path = Game::get_singleton(this)->get_songs_dir_path() + "/" +
                      editor_beatmap->get_beatmap_set_id();
    String map_id = editor_beatmap->get_beatmap_id();
    String map_path = set_path + "/" + map_id + map_extension;

    File* file = File::_new();
    dev_assert(file->open(map_path, File::WRITE) == Error::OK);

    editor_beatmap->write_contents(file);
    all_beatmaps[selected_beatmap_index]->copy(editor_beatmap);

    file->close();

    Godot::print("saved editor beatmap");
}

void MapManager::update_background_editor_beatmap(String image_path) {
    auto dir = Directory::_new();

    String set_path = Game::get_singleton(this)->get_songs_dir_path() + "/" +
                      editor_beatmap->get_beatmap_set_id();

    String image_filename = image_path.get_file();
    String new_image_path = set_path + "/" + image_filename;
    dev_assert(dir->copy(image_path, new_image_path) == Error::OK);

    editor_beatmap->set_background_filename(image_filename);
}
