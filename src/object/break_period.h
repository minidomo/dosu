#ifndef DOSU_BREAK_PERIOD_H
#define DOSU_BREAK_PERIOD_H

#include "../common.h"

class BreakPeriod {
   private:
    int start_time;
    int end_time;

   public:
    BreakPeriod(int start_time, int end_time);
    BreakPeriod();
    ~BreakPeriod();

    int get_start_time();
    void set_start_time(int start_time);

    int get_end_time();
    void set_end_time(int end_time);

    void parse_line(String line);
    String to_file_string();
};

#endif