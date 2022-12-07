#include "./main_menu.h"

#include "singleton/game.h"
#include "singleton/scene_manager.h"

void MainMenu::_register_methods() {
    dev_register_method(MainMenu, _ready);
    dev_register_method(MainMenu, on_play_button_pressed);
    dev_register_method(MainMenu, on_edit_button_pressed);
    dev_register_method(MainMenu, on_exit_button_pressed);
    dev_register_method(MainMenu, on_open_game_folder_button_pressed);
}

void MainMenu::_init() {}

void MainMenu::_ready() {
    Game::get_singleton(this)->set_borderless(false);
    Game::get_singleton(this)->set_confine_mouse(true);

    play_button = get_node<Button>("VBoxContainer/PlayButton");
    edit_button = get_node<Button>("VBoxContainer/EditButton");
    exit_button = get_node<Button>("VBoxContainer/ExitButton");
    open_game_folder_button =
        get_node<Button>("VBoxContainer/OpenGameFolderButton");

    play_button->connect("pressed", this, "on_play_button_pressed");
    edit_button->connect("pressed", this, "on_edit_button_pressed");
    exit_button->connect("pressed", this, "on_exit_button_pressed");
    open_game_folder_button->connect("pressed", this,
                                     "on_open_game_folder_button_pressed");
}

void MainMenu::on_play_button_pressed() {
    SceneManager::get_singleton(this)->to_play_scene();
}

void MainMenu::on_edit_button_pressed() {
    SceneManager::get_singleton(this)->to_edit_scene();
}

void MainMenu::on_exit_button_pressed() {
    SceneManager::get_singleton(this)->exit();
}

void MainMenu::on_open_game_folder_button_pressed() {
    Game::get_singleton(this)->open_directory("user://.", false);
}
