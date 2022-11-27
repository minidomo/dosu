#ifndef dosu_file_util
#define dosu_file_util

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
};

#endif