#ifndef dosu_file_break_period
#define dosu_file_break_period

#include "common/common.h"

class BreakPeriod {
   private:
    int64_t start_time;
    int64_t end_time;

   public:
    BreakPeriod(int64_t start_time, int64_t end_time);
    BreakPeriod();
    ~BreakPeriod();

    int64_t get_start_time();
    void set_start_time(int64_t start_time);

    int64_t get_end_time();
    void set_end_time(int64_t end_time);

    void parse_line(String line);
    String to_file_string();
};

#endif