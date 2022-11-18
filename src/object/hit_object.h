#ifndef DOSU_HIT_OBJECT_H
#define DOSU_HIT_OBJECT_H

#include "../common.h"
#include "./slider_data.h"

class HitObject {
   private:
    int64_t hit_type;
    int64_t start_x;
    int64_t start_y;
    int64_t start_time;

    /* normal, slider */
    bool new_combo;

    /* spinner */
    int64_t end_time;

    /* slider */
    SliderData slider_data;

   public:
    HitObject();
    ~HitObject();

    void parse_line(String line);
    String to_file_string();
};

#endif