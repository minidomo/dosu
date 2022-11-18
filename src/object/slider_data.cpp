#include "./slider_data.h"

SliderData::SliderData() {}

SliderData::~SliderData() {}

void SliderData::parse_array(PoolStringArray data) {
    repeats = data[1].to_int();
    repeats = Math::max(0LL, repeats - 1);

    int64_t length = data[2].to_int();

    String path_string = data[0];
    convert_path_string(path_string);
}

void SliderData::convert_path_string(String path_string) {}
