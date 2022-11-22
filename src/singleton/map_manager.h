#ifndef dosu_file_map_manager
#define dosu_file_map_manager

#include <Node.hpp>
#include <RandomNumberGenerator.hpp>
#include <vector>

#include "common/common.h"
#include "object/beatmap.h"

class MapManager : public Node {
    GODOT_CLASS(MapManager, Node);

   private:
    RandomNumberGenerator* random;

    vector<Beatmap> all_beatmaps;
    int64_t selected_beatmap_index;

    Array audio_extensions;
    String map_extension;

    String create_unique_set_id();
    String create_unique_map_id(String set_id);

    String extract_audio_extension(String file_path);

   public:
    static MapManager* get_singleton(Node* node);
    static void _register_methods();
    void _init();

    void _ready();

    void create_map(String set_id, String audio_filename);
    void create_set(String audio_path);

    bool is_valid_audio_extension(String file_path);

    void load_beatmaps();
    vector<Beatmap> get_all_beatmaps();

    void randomize_selected_beatmap_index();
    void set_selected_beatmap_index(int64_t selected_beatmap_index);
    int64_t get_selected_beatmap_index();
};

#endif