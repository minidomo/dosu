#include "./main_editor.h"

void MainEditor::_register_methods() {
    register_method("_ready", &MainEditor::_ready);
    register_method("on_tab_clicked", &MainEditor::on_tab_clicked);
}

void MainEditor::_init() { tab_index = -1; }

void MainEditor::_ready() {
    conductor = get_node<Conductor>("Conductor");

    init_bodies();
    init_tabs();

    on_tab_clicked(2);
}

void MainEditor::init_bodies() {
    Node *body_container = get_node("Body");
    Array children = body_container->get_children();

    for (int i = 0; i < children.size(); i++) {
        Control *body = Object::cast_to<Control>(children[i]);
        bodies.push_back(body);
    }
}

void MainEditor::init_tabs() {
    Node *tab_container = get_node("TopBar/Tabs");
    Array children = tab_container->get_children();

    for (int i = 0; i < children.size(); i++) {
        Tab *tab = Object::cast_to<Tab>(children[i]);
        tabs.push_back(tab);

        tab->get_button()->connect("pressed", this, "on_tab_clicked",
                                   Array::make(i));
    }
}

void MainEditor::on_tab_clicked(int index) {
    if (index == tab_index) return;
    tab_index = index;

    for (int i = 0; i < tabs.size(); i++) {
        if (i == index) {
            tabs[i]->select();
            bodies[i]->set_visible(true);
        } else {
            tabs[i]->deselect();
            bodies[i]->set_visible(false);
        }
    }
}