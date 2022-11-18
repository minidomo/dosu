#ifndef dosu_file_slider_data
#define dosu_file_slider_data

#include "./slider_path.h"
#include "common/common.h"

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