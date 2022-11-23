#include "./util.h"

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
    return (int64_t)Math::round(seconds * 1000);
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