#include "./editable_row.h"

void EditableRow::_register_methods() {
    register_method("_ready", &EditableRow::_ready);

    register_property<EditableRow, String>("title", &EditableRow::title, "");
}

void EditableRow::_init() { title = ""; }

void EditableRow::_ready() {
    label = get_node<Label>("Label");
    line_edit = get_node<LineEdit>("LineEdit");

    label->set_text(title);
}

String EditableRow::get_value() { return line_edit->get_text(); }

void EditableRow::set_value(String value) { line_edit->set_text(value); }

LineEdit* EditableRow::get_line_edit() { return line_edit; }