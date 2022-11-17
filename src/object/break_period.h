#ifndef DOSU_BREAK_PERIOD_H
#define DOSU_BREAK_PERIOD_H

#include "../common.h"

class BreakPeriod {
   private:
    int start;
    int end;

   public:
    BreakPeriod(int start, int end);
    BreakPeriod();
    ~BreakPeriod();

    int get_start();
    void set_start(int start);

    int get_end();
    void set_end(int end);

    String to_file_string();
};

#endif