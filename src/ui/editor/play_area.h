#ifndef dosu_file_play_area
#define dosu_file_play_area

#include <Control.hpp>
#include <PackedScene.hpp>
#include <Ref.hpp>
#include <vector>

#include "common/common.h"
#include "object/conductor.h"
#include "object/game/circle.h"
#include "object/hit_object.h"

class PlayArea : public Control {
    GODOT_CLASS(PlayArea, Control);

   private:
    Conductor *conductor;
    Control *circle_container;

    Ref<PackedScene> circle_object;

    void setup_circle(Circle *circle, HitObject *circle_data);
    void draw_hit_objects(vector<HitObject *> hit_objects);
    void draw_circle_objects(vector<HitObject *> hit_objects);

   public:
    static void _register_methods();
    void _init();

    void _ready();

    Vector2 convert_to_internal(Vector2 external_coordinate);
    Vector2 convert_to_external(Vector2 internal_coordinate);

    void set_conductor(Conductor *conductor);
    Conductor *get_conductor();

    void on_song_position_updated(float song_position);
    void on_approach_rate_updated();
    void on_circle_size_updated();
};

#endif