#ifndef dosu_file_slidable_row
#define dosu_file_slidable_row

#include <Control.hpp>
#include <HSlider.hpp>
#include <Label.hpp>

#include "common/common.h"

class SlidableRow : public Control {
    GODOT_CLASS(SlidableRow, Control);

   private:
    String title;
    String description;

    Label *title_label;
    Label *description_label;
    Label *value_label;
    HSlider *slider;

   public:
    static void _register_methods();
    void _init();

    void _ready();

    void on_value_changed(float value);

    float get_value();
    void set_value(float value);

    HSlider *get_slider();
};

#endif