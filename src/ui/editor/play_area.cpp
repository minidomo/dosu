#include "./play_area.h"

#include "singleton/map_manager.h"

void PlayArea::_register_methods() { dev_register_method(PlayArea, _ready); }

void PlayArea::_init() {}

void PlayArea::_ready() {}

Vector2 PlayArea::convert_to_internal(Vector2 external_coordinate) {
    Vector2 dimensions = get_size();

    Vector2 ret;

    ret.x = external_coordinate.x / dimensions.x *
            MapManager::get_singleton(this)->get_max_placeable_x_coordinate();
    ret.y = external_coordinate.y / dimensions.y *
            MapManager::get_singleton(this)->get_max_placeable_y_coordinate();

    ret.x = Math::floor(ret.x);
    ret.y = Math::floor(ret.y);

    return ret;
}

Vector2 PlayArea::convert_to_external(Vector2 internal_coordinate) {
    Vector2 dimensions = get_size();

    Vector2 ret;

    ret.x = internal_coordinate.x /
            MapManager::get_singleton(this)->get_max_placeable_x_coordinate() *
            dimensions.x;
    ret.y = internal_coordinate.y /
            MapManager::get_singleton(this)->get_max_placeable_y_coordinate() *
            dimensions.y;

    return ret;
}
