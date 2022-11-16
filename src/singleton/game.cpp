#include "./game.h"

Game* Game::get_singleton(Node* node) {
    return node->get_node<Game>("/root/Game");
}

void Game::_register_methods() { register_method("_ready", &Game::_ready); }

void Game::_init() {}

void Game::_ready() { Godot::print("game ready"); }
