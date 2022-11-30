#include "./break_period.h"

void BreakPeriod::_register_methods() {}

void BreakPeriod::_init() {
    start_time = 0;
    end_time = 0;
}

int64_t BreakPeriod::get_start_time() { return start_time; }

void BreakPeriod::set_start_time(int64_t start_time) {
    this->start_time = start_time;
}

int64_t BreakPeriod::get_end_time() { return end_time; }

void BreakPeriod::set_end_time(int64_t end_time) { this->end_time = end_time; }

void BreakPeriod::parse_line(String line) {
    auto arr = line.split(",");
    start_time = arr[1].to_int();
    end_time = arr[2].to_int();
}

String BreakPeriod::to_file_string() {
    return "2," + String::num_int64(start_time) + "," +
           String::num_int64(end_time);
}

void BreakPeriod::copy(BreakPeriod *break_period) {
    start_time = break_period->start_time;
    end_time = break_period->end_time;
}