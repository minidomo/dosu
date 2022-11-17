#include "./slider_data.h"

SliderData::SliderData() {}

SliderData::~SliderData() {}

void SliderData::parse_array(PoolStringArray data) {
    repeats = data[1].to_int();

    String path_string = data[0];
}