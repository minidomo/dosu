#ifndef dosu_file_break_period
#define dosu_file_break_period

#include <Node.hpp>

#include "common/common.h"

class BreakPeriod : public Node {
    GODOT_CLASS(BreakPeriod, Node);

   private:
    int64_t start_time;
    int64_t end_time;

   public:
    static void _register_methods();
    void _init();

    int64_t get_start_time();
    void set_start_time(int64_t start_time);

    int64_t get_end_time();
    void set_end_time(int64_t end_time);

    void parse_line(String line);
    String to_file_string();

    void copy(BreakPeriod *break_period);
};

#endif