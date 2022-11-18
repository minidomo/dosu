#include "./main_menu.h"

#include "singleton/game.h"
#include "singleton/scene_manager.h"

void MainMenu::_register_methods() {
    register_method("_ready", &MainMenu::_ready);

    register_method("on_play_button_pressed",
                    &MainMenu::on_play_button_pressed);
    register_method("on_edit_button_pressed",
                    &MainMenu::on_edit_button_pressed);
    register_method("on_exit_button_pressed",
                    &MainMenu::on_exit_button_pressed);

    register_property<MainMenu, NodePath>(
        "play_button_path", &MainMenu::play_button_path, NodePath());
    register_property<MainMenu, NodePath>(
        "edit_button_path", &MainMenu::edit_button_path, NodePath());
    register_property<MainMenu, NodePath>(
        "exit_button_path", &MainMenu::exit_button_path, NodePath());
}

void MainMenu::_init() {
    play_button_path = NodePath();
    edit_button_path = NodePath();
    exit_button_path = NodePath();
}

void MainMenu::_ready() {
    dev_assert(!play_button_path.is_empty());
    dev_assert(!edit_button_path.is_empty());
    dev_assert(!exit_button_path.is_empty());

    Game::get_singleton(this)->set_borderless(false);
    Game::get_singleton(this)->set_confine_mouse(true);

    play_button = get_node<Button>(play_button_path);
    edit_button = get_node<Button>(edit_button_path);
    exit_button = get_node<Button>(exit_button_path);

    play_button->connect("pressed", this, "on_play_button_pressed");
    edit_button->connect("pressed", this, "on_edit_button_pressed");
    exit_button->connect("pressed", this, "on_exit_button_pressed");
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
