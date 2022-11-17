#ifndef DOSU_SLIDER_DATA_H
#define DOSU_SLIDER_DATA_H

#include "../common.h"

class SliderData {
   private:
    int repeats;

   public:
    SliderData();
    ~SliderData();

    void parse_array(PoolStringArray data);
};

#endif