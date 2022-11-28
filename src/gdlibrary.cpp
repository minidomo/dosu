#include "common/index.h"
#include "object/index.h"
#include "singleton/index.h"
#include "ui/index.h"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT
godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
    Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    Godot::nativescript_init(handle);

    // object
    register_class<Circle>();
    register_class<Beatmap>();
    register_class<BreakPeriod>();
    register_class<TimingPoint>();
    register_class<HitObject>();
    register_class<Conductor>();

    // singleton
    register_class<Game>();
    register_class<MapManager>();
    register_class<SceneManager>();

    // ui
    register_class<PlayArea>();
    register_class<ComposeBody>();
    register_class<ObjectTimeline>();
    register_class<TimelineTick>();
    register_class<SongSetupBody>();
    register_class<SlidableRow>();
    register_class<EditableRow>();
    register_class<Timeline>();
    register_class<Tab>();
    register_class<Background>();
    register_class<BeatmapListing>();
    register_class<MenuFileButton>();
    register_class<MainEditor>();
    register_class<MainEdit>();
    register_class<MainMenu>();
}
