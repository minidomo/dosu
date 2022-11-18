#ifndef dosu_file_scene_manager
#define dosu_file_scene_manager

#include <Node.hpp>

#include "common/common.h"

class SceneManager : public Node {
    GODOT_CLASS(SceneManager, Node);

   private:
   public:
    static SceneManager* get_singleton(Node* node);
    static void _register_methods();
    void _init();

    void _ready();

    void exit();
    void to_edit_scene();
    void to_play_scene();
    void to_main_menu_scene();
    void to_editor_scene();
};

#endif