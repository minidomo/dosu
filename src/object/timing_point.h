#ifndef DOSU_TIMING_POINT_H
#define DOSU_TIMING_POINT_H

#include "../common.h"

class TimingPoint {
   private:
    int64_t time;
    float beat_length;
    int64_t meter;
    bool uninherited;

    /* green lines */
    float bpm_multiplier;
    float slider_velocity;

    /* red lines */
    float bpm;

   public:
    TimingPoint();
    ~TimingPoint();

    void parse_line(String line);
    String to_file_string();
};

#endif