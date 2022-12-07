#include "./menu_file_button.h"

#include <PopupMenu.hpp>

#include "object/beatmap.h"
#include "singleton/game.h"
#include "singleton/map_manager.h"
#include "singleton/scene_manager.h"

void MenuFileButton::_register_methods() {
    register_method("_ready", &MenuFileButton::_ready);
    register_method("on_index_pressed", &MenuFileButton::on_index_pressed);
}

void MenuFileButton::_init() {}

void MenuFileButton::_ready() {
    auto popup = get_popup();

    popup->add_item("Open Directory");
    popup->add_item("Export");
    popup->add_item("Save");
    popup->add_item("Exit");

    popup->connect("id_pressed", this, "on_index_pressed");
}

void MenuFileButton::on_index_pressed(int64_t index) {
    auto popup = get_popup();
    String text = popup->get_item_text(index);

    if (text == "Exit") {
        SceneManager::get_singleton(this)->to_edit_scene();
    } else if (text == "Save") {
        MapManager::get_singleton(this)->save_editor_beatmap();
    } else if (text == "Open Directory") {
        auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
        String path = Beatmap::get_dir_path(this, beatmap);
        Game::get_singleton(this)->open_directory(path, false);
    } else if (text == "Export") {
        auto game = Game::get_singleton(this);
        auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
        game->export_beatmap(beatmap);
        game->open_directory(game->get_export_dir_path(), false);
    }
}