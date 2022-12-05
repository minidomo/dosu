#ifndef dosu_file_beat_snap_divisor
#define dosu_file_beat_snap_divisor

#include <Control.hpp>
#include <HSlider.hpp>
#include <Label.hpp>

#include "common/common.h"

class BeatSnapDivisor : public Control {
    GODOT_CLASS(BeatSnapDivisor, Control);

   private:
    Label *value_label;
    HSlider *slider;

    int64_t value;

    int64_t round_value(float value);

   public:
    static void _register_methods();
    void _init();
    void _ready();

    void set_value(int64_t value);
    int64_t get_value();

    void on_value_changed(float value);

    void initialize();
};

#endif