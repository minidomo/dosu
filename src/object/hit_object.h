#ifndef DOSU_HIT_OBJECT_H
#define DOSU_HIT_OBJECT_H

#include "../common.h"
#include "./slider_data.h"

class HitObject {
   private:
    int hit_type;
    int start_x;
    int start_y;
    int start_time;

    /* normal, slider */
    bool new_combo;

    /* spinner */
    int end_time;

    /* slider */
    SliderData slider_data;

   public:
    HitObject();
    ~HitObject();

    void parse_line(String line);
    String to_file_string();
};

#endif