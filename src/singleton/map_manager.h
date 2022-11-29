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
    int64_t fade_out_time;

    int64_t min_placeable_x_coordinate;
    int64_t min_placeable_y_coordinate;
    int64_t max_placeable_x_coordinate;
    int64_t max_placeable_y_coordinate;

    RandomNumberGenerator* random;

    Beatmap* editor_beatmap;
    vector<Beatmap*> all_beatmaps;
    int64_t selected_beatmap_index;

    Array audio_extensions;
    Array image_extensions;
    String map_extension;

    String create_unique_set_id();
    String create_unique_map_id(String set_id);

    String extract_audio_extension(String file_path);

    float circle_size_to_pixel_for_1080(float circle_size);

   public:
    static MapManager* get_singleton(Node* node);
    static void _register_methods();
    void _init();

    void _ready();

    void create_map(String set_id, String audio_filename);
    void create_set(String audio_path);

    String get_map_extension();
    bool is_valid_audio_extension(String file_path);
    bool is_valid_image_extension(String file_path);

    void load_beatmaps();
    vector<Beatmap*> get_all_beatmaps();

    void randomize_selected_beatmap_index();
    void set_selected_beatmap_index(int64_t selected_beatmap_index);
    int64_t get_selected_beatmap_index();

    Beatmap* get_editor_beatmap();
    void refresh_editor_beatmap();
    void save_editor_beatmap();
    void update_background_editor_beatmap(String image_path);

    int64_t get_min_placeable_x_coordinate();
    int64_t get_min_placeable_y_coordinate();
    int64_t get_max_placeable_x_coordinate();
    int64_t get_max_placeable_y_coordinate();
    int64_t approach_rate_to_ms(float approach_rate);
    int64_t get_fade_out_time();
    float circle_size_to_pixel(float circle_size);
};

#endif