#include "./compose_body.h"

#include "common/util.h"
#include "object/enum/hit_type.h"
#include "object/hit_object.h"
#include "singleton/map_manager.h"

void ComposeBody::_register_methods() {
    dev_register_method(ComposeBody, _ready);
    dev_register_method(ComposeBody, on_select_button_pressed);
    dev_register_method(ComposeBody, on_circle_button_pressed);
    dev_register_method(ComposeBody, on_delete_button_pressed);
}

void ComposeBody::_init() {}

void ComposeBody::_ready() {
    select_button = get_node<BaseButton>("LeftBar/SelectButton");
    circle_button = get_node<BaseButton>("LeftBar/CircleButton");
    delete_button = get_node<BaseButton>("LeftBar/DeleteButton");
    play_area = get_node<PlayArea>("PlayArea");

    select_button->connect("pressed", this, "on_select_button_pressed");
    circle_button->connect("pressed", this, "on_circle_button_pressed");
    delete_button->connect("pressed", this, "on_delete_button_pressed");
}

void ComposeBody::on_circle_button_pressed() {
    auto conductor = play_area->get_conductor();
    auto map_manager = MapManager::get_singleton(this);
    auto beatmap = map_manager->get_editor_beatmap();

    auto hit_object = HitObject::_new();
    hit_object->set_start_x(map_manager->get_max_placeable_x_coordinate() / 2);
    hit_object->set_start_y(map_manager->get_max_placeable_y_coordinate() / 2);
    hit_object->set_hit_type(+HitType::Normal);
    hit_object->set_start_time(
        Util::to_milliseconds(conductor->get_song_position()));
    hit_object->set_hit_sound(1);

    beatmap->add_hit_object(hit_object);
}

PlayArea* ComposeBody::get_play_area() { return play_area; }

void ComposeBody::on_select_button_pressed() {}

void ComposeBody::on_delete_button_pressed() {
    auto conductor = play_area->get_conductor();
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();

    int64_t time = Util::to_milliseconds(conductor->get_song_position());
    beatmap->remove_hit_object(time);
}