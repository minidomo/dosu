#ifndef dosu_file_timing_body_input_entry
#define dosu_file_timing_body_input_entry

#include <Control.hpp>
#include <LineEdit.hpp>

#include "common/common.h"

class TimingBodyInputEntry : public Control {
    GODOT_CLASS(TimingBodyInputEntry, Control);

   private:
    String value;
    LineEdit *input;

   public:
    static void _register_methods();
    void _init();
    void _ready();

    LineEdit *get_input();
    void set_value(String value);
    String get_value();
};

#endif