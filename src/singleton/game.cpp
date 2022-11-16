#include "./game.h"

#include <Input.hpp>
#include <OS.hpp>

Game* Game::get_singleton(Node* node) {
    return node->get_node<Game>("/root/Game");
}

void Game::_register_methods() { register_method("_ready", &Game::_ready); }

void Game::_init() {}

void Game::_ready() {
    Godot::print("game ready");
    Godot::print("" + OS::get_singleton()->get_window_size());
    Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CONFINED);
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