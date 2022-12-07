#include "./timing_body_input_entry.h"

void TimingBodyInputEntry::_register_methods() {
    dev_register_method(TimingBodyInputEntry, _ready);
}

void TimingBodyInputEntry::_init() {}

void TimingBodyInputEntry::_ready() { input = get_node<LineEdit>("Value"); }

LineEdit* TimingBodyInputEntry::get_input() { return input; }