#ifndef DOSU_SCENE_MANAGER_H
#define DOSU_SCENE_MANAGER_H

#include <Node.hpp>

#include "../common.h"

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
};

#endif