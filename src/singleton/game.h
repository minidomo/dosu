#ifndef dosu_file_game
#define dosu_file_game

#include <Node.hpp>

#include "common/common.h"
#include "object/beatmap.h"

class Game : public Node {
    GODOT_CLASS(Game, Node);

   private:
    String default_background_path;
    String songs_dir_path;
    String export_dir_path;
    String export_extension;

    void init_directory(String path);

   public:
    static Game* get_singleton(Node* node);
    static void _register_methods();
    void _init();

    void _ready();

    void set_borderless(bool borderless);
    void set_confine_mouse(bool confine);

    String get_songs_dir_path();
    String get_export_dir_path();
    String get_default_background_path();

    void open_directory(String path, bool is_global);
    void export_beatmap(Beatmap* beatmap);
};

#endif