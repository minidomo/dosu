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
    register_class<Conductor>();

    // singleton
    register_class<Game>();
    register_class<MapManager>();
    register_class<SceneManager>();

    // ui
    register_class<Background>();
    register_class<BeatmapListing>();
    register_class<MenuFileButton>();
    register_class<MainEditor>();
    register_class<MainEdit>();
    register_class<MainMenu>();
}
