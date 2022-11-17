#ifndef DOSU_TIMING_POINT_H
#define DOSU_TIMING_POINT_H

#include "../common.h"

class TimingPoint {
   private:
    int time;
    float beat_length;
    int meter;
    bool uninherited;

    float bpm_multiplier;
    float slider_velocity;

   public:
    TimingPoint();
    ~TimingPoint();

    void parse_line(String line);
    String to_file_string();
};

#endif