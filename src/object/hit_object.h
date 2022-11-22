#ifndef dosu_file_hit_object
#define dosu_file_hit_object

#include "./slider_data.h"
#include "common/common.h"

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

    void copy(HitObject hit_object);
};

#endif