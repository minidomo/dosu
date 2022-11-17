#include "./break_period.h"

BreakPeriod::BreakPeriod(int start_time, int end_time) {
    this->start_time = start_time;
    this->end_time = end_time;
}

BreakPeriod::BreakPeriod() {}

BreakPeriod::~BreakPeriod() {}

int BreakPeriod::get_start_time() { return start_time; }

void BreakPeriod::set_start_time(int start_time) {
    this->start_time = start_time;
}

int BreakPeriod::get_end_time() { return end_time; }

void BreakPeriod::set_end_time(int end_time) { this->end_time = end_time; }

void BreakPeriod::parse_line(String line) {
    auto arr = line.split(",");
    start_time = arr[1].to_int();
    end_time = arr[2].to_int();
}

String BreakPeriod::to_file_string() {
    return "2," + String::num_int64(start_time) + "," +
           String::num_int64(end_time);
}
