#ifndef dosu_file_editable_row
#define dosu_file_editable_row

#include <Control.hpp>
#include <Label.hpp>
#include <LineEdit.hpp>

#include "common/common.h"

class EditableRow : public Control {
    GODOT_CLASS(EditableRow, Control);

   private:
    String title;

    Label *label;
    LineEdit *line_edit;

   public:
    static void _register_methods();
    void _init();

    void _ready();

    String get_value();
    void set_value(String value);

    LineEdit *get_line_edit();
};

#endif