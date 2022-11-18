#ifndef DOSU_SLIDER_DATA_H
#define DOSU_SLIDER_DATA_H

#include "../common.h"
#include "./slider_path.h"

class SliderData {
   private:
    int64_t repeats;
    SliderPath path;

    void convert_path_string(String path_string);

   public:
    SliderData();
    ~SliderData();

    void parse_array(PoolStringArray data);
};

#endif