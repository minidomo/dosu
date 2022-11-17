#ifndef DOSU_TIMING_POINT_H
#define DOSU_TIMING_POINT_H

#include "../common.h"

class TimingPoint {
   private:
   public:
    TimingPoint();
    ~TimingPoint();

    String to_file_string();
};

#endif