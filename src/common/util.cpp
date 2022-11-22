#include "./util.h"

void Util::delete_children(Node *node) {
    Array children = node->get_children();
    for (int i = 0; i < children.size(); i++) {
        Node *child = Object::cast_to<Node>(children[i]);
        node->remove_child(child);
        child->queue_free();
    }
}