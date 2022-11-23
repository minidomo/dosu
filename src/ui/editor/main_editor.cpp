#include "./main_editor.h"

#include <AudioStream.hpp>
#include <AudioStreamMP3.hpp>
#include <BaseButton.hpp>
#include <File.hpp>
#include <Ref.hpp>
#include <SceneTree.hpp>

#include "common/util.h"
#include "object/enum/conductor_go_type.h"
#include "object/enum/icon_type.h"
#include "singleton/game.h"
#include "singleton/map_manager.h"

void MainEditor::_register_methods() {
    register_method("_ready", &MainEditor::_ready);
    register_method("on_tab_clicked", &MainEditor::on_tab_clicked);
    register_method("on_icon_button_pressed",
                    &MainEditor::on_icon_button_pressed);
    register_method("on_song_position_update",
                    &MainEditor::on_song_position_update);
    register_method("on_timeline_click", &MainEditor::on_timeline_click);
    register_method("on_files_dropped", &MainEditor::on_files_dropped);
}

void MainEditor::_init() { tab_index = -1; }

void MainEditor::_ready() {
    MapManager::get_singleton(this)->refresh_editor_beatmap();

    background = get_node<Background>("Background");
    conductor = get_node<Conductor>("Conductor");
    time_label = get_node<Label>("BottomBar/Time/Label");
    progress_label = get_node<Label>("BottomBar/Progress/Label");
    timeline = get_node<Timeline>("BottomBar/Timeline");

    timeline->connect("timeline_click", this, "on_timeline_click");
    conductor->connect("song_position_update", this, "on_song_position_update");
    get_tree()->connect("files_dropped", this, "on_files_dropped");

    init_conductor();
    init_bodies();
    init_tabs();
    init_icon_buttons();

    on_tab_clicked(2);
    Background::update_background(
        background, MapManager::get_singleton(this)->get_editor_beatmap());
}

void MainEditor::init_bodies() {
    Node *body_container = get_node("Body");
    Array children = body_container->get_children();

    for (int i = 0; i < children.size(); i++) {
        Control *body = Object::cast_to<Control>(children[i]);
        bodies.push_back(body);
    }
}

void MainEditor::init_tabs() {
    Node *tab_container = get_node("TopBar/Tabs");
    Array children = tab_container->get_children();

    for (int i = 0; i < children.size(); i++) {
        Tab *tab = Object::cast_to<Tab>(children[i]);
        tabs.push_back(tab);

        tab->get_button()->connect("pressed", this, "on_tab_clicked",
                                   Array::make(i));
    }
}

void MainEditor::on_tab_clicked(int index) {
    if (index == tab_index) return;
    tab_index = index;

    for (int i = 0; i < tabs.size(); i++) {
        if (i == index) {
            tabs[i]->select();
            bodies[i]->set_visible(true);
        } else {
            tabs[i]->deselect();
            bodies[i]->set_visible(false);
        }
    }
}

void MainEditor::init_icon_buttons() {
    Node *container = get_node("BottomBar/IconButtons");
    Array children = container->get_children();

    for (int i = 0; i < children.size(); i++) {
        auto *button = Object::cast_to<BaseButton>(children[i]);
        button->connect("pressed", this, "on_icon_button_pressed",
                        Array::make(i));
    }
}

void MainEditor::on_icon_button_pressed(int index) {
    switch (index) {
        case +IconType::SeekStartPlay: {
            conductor->go_to(0, ConductorGoType::Play);
            break;
        }
        case +IconType::TogglePause: {
            conductor->toggle_pause();
            break;
        }
        case +IconType::SeekStartPause: {
            conductor->go_to(0, ConductorGoType::Pause);
            break;
        }
        default: {
            dev_assert(false);
            break;
        }
    }
}

void MainEditor::init_conductor() {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();

    // get audio stream for conductor
    String path = Game::get_singleton(this)->get_songs_dir_path() + "/" +
                  beatmap.get_beatmap_set_id() + "/" +
                  beatmap.get_audio_filename();

    auto file = File::_new();
    dev_assert(file->open(path, File::READ) == Error::OK);

    auto bytes = file->get_buffer(file->get_len());

    auto audio_stream = AudioStreamMP3::_new();
    audio_stream->set_data(bytes);
    Ref<AudioStream> ref_audio_stream(audio_stream);

    conductor->set_stream(ref_audio_stream);
}

void MainEditor::on_song_position_update(int64_t song_position) {
    time_label->set_text(Util::to_timestamp(song_position));

    float percent = (float)song_position / conductor->get_total_duration();
    String progress = String::num_real(percent * 100.f).pad_decimals(1) + "%";

    progress_label->set_text(progress);
    timeline->set_playhead_progress(percent);
}

void MainEditor::on_timeline_click(float percent) {
    conductor->go_to_percent(percent, ConductorGoType::Maintain);
}

void MainEditor::on_files_dropped(PoolStringArray files, int screen) {
    String path = files[0];

    auto map_manager = MapManager::get_singleton(this);

    if (map_manager->is_valid_image_extension(path)) {
        map_manager->update_background_editor_beatmap(path);
        Background::update_background(background,
                                      map_manager->get_editor_beatmap());
    } else {
        // TODO display error?
    }
}