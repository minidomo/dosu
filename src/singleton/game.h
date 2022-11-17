#ifndef DOSU_GAME_H
#define DOSU_GAME_H

#include <Node.hpp>

#include "../common.h"

class Game : public Node {
    GODOT_CLASS(Game, Node);

   private:
    String songs_dir_path;

    void init_songs_directory();

   public:
    static Game* get_singleton(Node* node);
    static void _register_methods();
    void _init();

    void _ready();

    void set_borderless(bool borderless);
    void set_confine_mouse(bool confine);

    String get_songs_dir_path();
};

#endif