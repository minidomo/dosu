#include "./main_editor.h"

#include <AudioStream.hpp>
#include <AudioStreamMP3.hpp>
#include <BaseButton.hpp>
#include <File.hpp>
#include <Input.hpp>
#include <SceneTree.hpp>

#include "common/util.h"
#include "object/beatmap.h"
#include "object/enum/conductor_go_type.h"
#include "object/enum/icon_type.h"
#include "singleton/game.h"
#include "singleton/map_manager.h"

void MainEditor::_register_methods() {
    dev_register_method(MainEditor, _ready);
    dev_register_method(MainEditor, _process);
    dev_register_method(MainEditor, on_tab_clicked);
    dev_register_method(MainEditor, on_icon_button_pressed);
    dev_register_method(MainEditor, on_song_position_updated);
    dev_register_method(MainEditor, on_timeline_click);
    dev_register_method(MainEditor, on_files_dropped);
    dev_register_method(MainEditor, on_timeline_zoom_button_pressed);
}

void MainEditor::_init() { tab_index = -1; }

void MainEditor::_ready() {
    /* obtaining initial nodes */
    background = get_node<Background>("Background");
    conductor = get_node<Conductor>("Conductor");
    time_label = get_node<Label>("BottomBar/Time/Label");
    progress_label = get_node<Label>("BottomBar/Progress/Label");
    timeline = get_node<Timeline>("BottomBar/Timeline");
    song_setup_body = get_node<SongSetupBody>("Body/SongSetupBody");
    compose_body = get_node<ComposeBody>("Body/ComposeBody");
    object_timeline = get_node<ObjectTimeline>("TopBar/ObjectTimeline");
    play_area = compose_body->get_play_area();

    /* connects */

    timeline->connect("timeline_click", this, "on_timeline_click");
    conductor->connect("song_position_updated", this,
                       "on_song_position_updated");
    get_tree()->connect("files_dropped", this, "on_files_dropped");

    /* init stuff */

    init_conductor();
    init_bodies();
    init_tabs();
    init_icon_buttons();
    init_timeline_zoom_buttons();

    /* final things */

    on_tab_clicked(2);

    background->set_background_path(Beatmap::get_background_file_path(
        this, MapManager::get_singleton(this)->get_editor_beatmap()));

    object_timeline->set_conductor(conductor);
    object_timeline->initialize();

    play_area->set_conductor(conductor);
    play_area->initialize();

    timeline->set_conductor(conductor);
    timeline->initialize();
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
                  beatmap->get_beatmap_set_id() + "/" +
                  beatmap->get_audio_filename();

    auto file = File::_new();
    dev_assert(file->open(path, File::READ) == Error::OK);

    auto bytes = file->get_buffer(file->get_len());

    auto audio_stream = AudioStreamMP3::_new();
    audio_stream->set_data(bytes);
    Ref<AudioStream> ref_audio_stream(audio_stream);

    conductor->set_stream(ref_audio_stream);
    conductor->set_bpm(beatmap->get_timing_points()[0]->get_bpm());
}

void MainEditor::on_song_position_updated(float song_position) {
    time_label->set_text(
        Util::to_timestamp(Util::to_milliseconds(song_position)));

    float percent = song_position / conductor->get_total_duration();
    String progress = String::num_real(percent * 100).pad_decimals(1) + "%";

    progress_label->set_text(progress);
}

void MainEditor::on_timeline_click(float percent) {
    conductor->go_to_percent(percent, ConductorGoType::Maintain);
}

void MainEditor::on_files_dropped(PoolStringArray files, int screen) {
    String path = files[0];

    auto map_manager = MapManager::get_singleton(this);

    if (map_manager->is_valid_image_extension(path)) {
        map_manager->update_background_editor_beatmap(path);
        background->set_background_path(Beatmap::get_background_file_path(
            this, map_manager->get_editor_beatmap()));
    } else {
        // TODO display error?
    }
}

void MainEditor::_process(float delta) {
    auto input = Input::get_singleton();

    if (input->is_action_just_released("scroll_up") ||
        input->is_action_just_released("scroll_down")) {
        bool scroll_up = input->is_action_just_released("scroll_up");

        if (input->is_action_pressed("alt")) {
            if (object_timeline->is_hovering()) {
                float value = scroll_up ? .1f : -.1f;
                on_timeline_zoom_button_pressed(value);
            }
        } else {
            float song_position = conductor->get_song_position();
            float target_time = 0;

            if (conductor->is_actually_playing()) {
                float sign = scroll_up ? -1.f : 1.f;
                float step = .5f * sign;
                target_time = Math::clamp<float>(
                    song_position + step, 0, conductor->get_total_duration());
            } else {
                auto beatmap =
                    MapManager::get_singleton(this)->get_editor_beatmap();
                auto control_point = beatmap->get_control_point_for_time(
                    Util::to_milliseconds(song_position));

                float song_offset = Util::to_seconds(control_point->get_time());
                int64_t beat_offset = scroll_up ? -1 : 1;

                Dictionary beat_info =
                    conductor->get_beat(song_position, song_offset, beat_offset,
                                        beatmap->get_beat_divisor());

                target_time = beat_info["accessible_time"];
            }

            conductor->go_to(target_time, ConductorGoType::Maintain);
        }
    }
}

void MainEditor::init_timeline_zoom_buttons() {
    Node *container = get_node("TopBar/TimelineZoomButtons");
    auto increase_button =
        container->get_node<BaseButton>("TimelineZoomIncrease");
    auto decrease_button =
        container->get_node<BaseButton>("TimelineZoomDecrease");

    increase_button->connect("pressed", this, "on_timeline_zoom_button_pressed",
                             Array::make(.1f));
    decrease_button->connect("pressed", this, "on_timeline_zoom_button_pressed",
                             Array::make(-.1f));
}

void MainEditor::on_timeline_zoom_button_pressed(float value) {
    auto beatmap = MapManager::get_singleton(this)->get_editor_beatmap();
    beatmap->set_timeline_zoom(
        Math::max(.1f, beatmap->get_timeline_zoom() + value));
}
