#ifndef dosu_file_util
#define dosu_file_util

#include <Control.hpp>
#include <Node.hpp>

#include "common/common.h"
class Util {
   public:
    static void delete_children(Node *node);
    static float ceil(float value);
    static float to_seconds(int64_t milliseconds);
    static int64_t to_milliseconds(float seconds);
    static String to_timestamp(int64_t milliseconds);
    static String pad_start(int64_t num, String value, int64_t len);
    static String limit_decimal(float value, int max_digits);
    static bool float_gte(float a, float b);
    static bool float_lte(float a, float b);
    static int64_t mod(int64_t value, int64_t mod);
    static float scale_value(float base_value, float base_reference,
                             float target_reference);
    static float scale_value_by_resolution(float base_value,
                                           float base_resolution);
    static void recursive_scale_font(Control *node, Dictionary ids);
};

#endif