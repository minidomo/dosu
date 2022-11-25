#ifndef dosu_file_hit_object
#define dosu_file_hit_object

#include <Node.hpp>

#include "common/common.h"

class HitObject : public Node {
    GODOT_CLASS(HitObject, Node);

   private:
    int64_t hit_type;
    int64_t start_x;
    int64_t start_y;
    int64_t start_time;

    /* normal, slider */
    bool new_combo;

    /* spinner */
    int64_t end_time;

   public:
    static void _register_methods();
    void _init();

    void parse_line(String line);
    String to_file_string();

    void copy(HitObject *hit_object);
};

#endif