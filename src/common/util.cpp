#include "./util.h"

#include <DynamicFont.hpp>
#include <OS.hpp>

void Util::delete_children(Node *node) {
    Array children = node->get_children();
    for (int i = 0; i < children.size(); i++) {
        Node *child = Object::cast_to<Node>(children[i]);
        node->remove_child(child);
        child->queue_free();
    }
}

float Util::ceil(float value) {
    if (Math::floor(value) == value) {
        return value;
    }

    return Math::floor(value + 1);
}

int64_t Util::to_milliseconds(float seconds) {
    return (int64_t)Math::floor(seconds * 1000);
}

String Util::to_timestamp(int64_t milliseconds) {
    int64_t remaining_ms = milliseconds % 1000;
    int64_t seconds = milliseconds / 1000;
    int64_t remaining_sec = seconds % 60;
    int64_t remaining_min = seconds / 60;

    String ret = pad_start(remaining_min, "0", 2) + ":" +
                 pad_start(remaining_sec, "0", 2) + ":" +
                 pad_start(remaining_ms, "0", 3);

    return ret;
}

String Util::pad_start(int64_t num, String value, int64_t len) {
    String ret = String::num_int64(num);

    for (int i = ret.length(); i < len; i++) {
        ret = value + ret;
    }

    return ret;
}

float Util::to_seconds(int64_t milliseconds) { return milliseconds / 1000.f; }

String Util::limit_decimal(float value, int max_digits) {
    if (value == (int)value) {
        return String::num_int64((int)value);
    }

    return String::num_real(value).pad_decimals(max_digits);
}

bool Util::float_gte(float a, float b) {
    return a > b || Math::is_equal_approx(a, b);
}

bool Util::float_lte(float a, float b) {
    return a < b || Math::is_equal_approx(a, b);
}

int64_t Util::mod(int64_t value, int64_t mod) {
    return (value % mod + mod) % mod;
}

float Util::scale_value(float base_value, float base_reference,
                        float target_reference) {
    return base_value / base_reference * target_reference;
}

float Util::scale_value_by_resolution(float base_value, float base_resolution) {
    return scale_value(base_value, base_resolution,
                       OS::get_singleton()->get_window_size().height);
}

void Util::recursive_scale_font(Control *node, Dictionary ids) {
    if (node == nullptr) return;

    auto font = Object::cast_to<DynamicFont>(node->get("custom_fonts/font"));
    if (font) {
        String key = String::num_int64(font->get_instance_id());
        if (!ids.has(key)) {
            ids[key] = true;
            int64_t cur_size = font->get_size();
            int64_t new_size =
                (int64_t)scale_value_by_resolution((float)cur_size, 1080);
            font->set_size(new_size);
        }
    }

    for (int i = 0; i < node->get_child_count(); i++) {
        auto next = Object::cast_to<Control>(node->get_child(i));
        recursive_scale_font(next, ids);
    }
}