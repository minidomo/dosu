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
    int64_t hit_sound;

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

    void set_hit_type(int64_t hit_type);
    void set_start_x(int64_t start_x);
    void set_start_y(int64_t start_y);
    void set_start_time(int64_t start_time);
    void set_hit_sound(int64_t hit_sound);
    void set_new_combo(bool new_combo);
    void set_end_time(int64_t end_time);

    int64_t get_hit_type();
    int64_t get_start_x();
    int64_t get_start_y();
    int64_t get_start_time();
    int64_t get_hit_sound();
    bool is_new_combo();
    int64_t get_end_time();

    bool is_normal();
    bool is_spinner();
    bool is_slider();
};

#endif