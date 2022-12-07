#ifndef dosu_file_timing_point_row
#define dosu_file_timing_point_row

#include <BaseButton.hpp>
#include <Control.hpp>
#include <Label.hpp>

#include "common/common.h"

class TimingPointRow : public Control {
    GODOT_CLASS(TimingPointRow, Control);

   private:
    float bpm;
    int64_t meter;
    int64_t time;
    bool selected;

    Control *background;
    Label *time_label;
    Label *metadata_label;
    BaseButton *button;

    void update_metadata();

   public:
    static void _register_methods();
    void _init();
    void _ready();

    void color_scheme_select();
    void color_scheme_deselect();

    void set_bpm(float bpm);
    void set_meter(int64_t meter);
    void set_time(int64_t time);

    float get_bpm();
    int64_t get_meter();
    int64_t get_time();
    BaseButton *get_button();
    bool is_selected();
};

#endif