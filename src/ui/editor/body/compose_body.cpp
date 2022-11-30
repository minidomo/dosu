#include "./compose_body.h"

#include "common/util.h"
#include "object/enum/hit_sound.h"
#include "object/enum/hit_type.h"
#include "object/hit_object.h"
#include "singleton/map_manager.h"

void ComposeBody::_register_methods() {
    dev_register_method(ComposeBody, _ready);
    dev_register_method(ComposeBody, on_select_button_pressed);
    dev_register_method(ComposeBody, on_circle_button_pressed);
    dev_register_method(ComposeBody, on_delete_button_pressed);
    dev_register_method(ComposeBody, on_none_button_pressed);
    dev_register_method(ComposeBody, on_whistle_button_pressed);
    dev_register_method(ComposeBody, on_finish_button_pressed);
    dev_register_method(ComposeBody, on_clap_button_pressed);
}

void ComposeBody::_init() {}

void ComposeBody::_ready() {
    select_button = get_node<BaseButton>("LeftBar/VBoxContainer/SelectButton");
    circle_button = get_node<BaseButton>("LeftBar/VBoxContainer/CircleButton");
    delete_button = get_node<BaseButton>("LeftBar/VBoxContainer/DeleteButton");

    none_button = get_node<BaseButton>("RightBar/VBoxContainer/NoneButton");
    whistle_button =
        get_node<BaseButton>("RightBar/VBoxContainer/WhistleButton");
    finish_button = get_node<BaseButton>("RightBar/VBoxContainer/FinishButton");
    clap_button = get_node<BaseButton>("RightBar/VBoxContainer/ClapButton");

    play_area = get_node<PlayArea>("PlayArea");

    select_button->connect("pressed", this, "on_select_button_pressed");
    circle_button->connect("pressed", this, "on_circle_button_pressed");
    delete_button->connect("pressed", this, "on_delete_button_pressed");
    none_button->connect("pressed", this, "on_none_button_pressed");
    whistle_button->connect("pressed", this, "on_whistle_button_pressed");
    finish_button->connect("pressed", this, "on_finish_button_pressed");
    clap_button->connect("pressed", this, "on_clap_button_pressed");
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
    hit_object->set_hit_sound(+HitSound::None);

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

void ComposeBody::on_none_button_pressed() {
    auto conductor = play_area->get_conductor();
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    auto hit_object = beatmap->find_hit_object(
        Util::to_milliseconds(conductor->get_song_position()));

    if (hit_object) {
        hit_object->set_hit_sound(+HitSound::None);
        beatmap->emit_signal("hit_objects_updated");
    }
}

void ComposeBody::on_whistle_button_pressed() {
    auto conductor = play_area->get_conductor();
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    auto hit_object = beatmap->find_hit_object(
        Util::to_milliseconds(conductor->get_song_position()));

    if (hit_object) {
        hit_object->set_hit_sound(+HitSound::Whistle);
        beatmap->emit_signal("hit_objects_updated");
    }
}

void ComposeBody::on_finish_button_pressed() {
    auto conductor = play_area->get_conductor();
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    auto hit_object = beatmap->find_hit_object(
        Util::to_milliseconds(conductor->get_song_position()));

    if (hit_object) {
        hit_object->set_hit_sound(+HitSound::Finish);
        beatmap->emit_signal("hit_objects_updated");
    }
}

void ComposeBody::on_clap_button_pressed() {
    auto conductor = play_area->get_conductor();
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    auto hit_object = beatmap->find_hit_object(
        Util::to_milliseconds(conductor->get_song_position()));

    if (hit_object) {
        hit_object->set_hit_sound(+HitSound::Clap);
        beatmap->emit_signal("hit_objects_updated");
    }
}