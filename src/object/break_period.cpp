#include "./break_period.h"

BreakPeriod::BreakPeriod(int start, int end) {
    this->start = start;
    this->end = end;
}

BreakPeriod::BreakPeriod() {}

BreakPeriod::~BreakPeriod() {}

int BreakPeriod::get_start() { return start; }

void BreakPeriod::set_start(int start) { this->start = start; }

int BreakPeriod::get_end() { return end; }

void BreakPeriod::set_end(int end) { this->end = end; }

String BreakPeriod::to_file_string() {
    return "2," + String::num_int64(start) + "," + String::num_int64(end);
}
