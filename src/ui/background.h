#ifndef dosu_file_background
#define dosu_file_background

#include <ColorRect.hpp>
#include <Control.hpp>
#include <TextureRect.hpp>

#include "common/common.h"

class Background : public Control {
    GODOT_CLASS(Background, Control);

   private:
    String background_path;
    float dim;

    TextureRect *background_image;
    ColorRect *dimmer;

    void update_background();

   public:
    static void _register_methods();
    void _init();

    void _ready();

    void set_background_path(String background_path);
    void set_dim(float dim);

    String get_background_path();
    float get_dim();
};

#endif