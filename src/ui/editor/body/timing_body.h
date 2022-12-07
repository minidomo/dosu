#ifndef dosu_file_timing_body
#define dosu_file_timing_body

#include <BaseButton.hpp>
#include <Control.hpp>
#include <PackedScene.hpp>
#include <Ref.hpp>

#include "common/common.h"
#include "object/conductor.h"
#include "object/timing_point.h"
#include "ui/editor/timing_body_input_entry.h"
#include "ui/editor/timing_point_row.h"

class TimingBody : public Control {
    GODOT_CLASS(TimingBody, Control);

   private:
    BaseButton *add_timing_point_button;
    BaseButton *delete_timing_point_button;
    Control *timing_point_row_container;
    BaseButton *current_time_button;
    TimingBodyInputEntry *time_input_entry;
    TimingBodyInputEntry *bpm_input_entry;
    TimingBodyInputEntry *time_signature_input_entry;

    Conductor *conductor;
    Ref<PackedScene> timing_point_row_object;

    int64_t select_time;

    void draw_timing_point_rows();
    void setup_timing_point_row(TimingPointRow *row, TimingPoint *data,
                                int index);
    void select_row(int index, bool seek);
    void deselect_all_rows();
    int find_selected_row_index();
    int find_index_for_time(int64_t time);

    void update_time(int64_t time);

   public:
    static void _register_methods();
    void _init();
    void _ready();

    void initialize();

    void set_conductor(Conductor *conductor);
    Conductor *get_conductor();

    void on_add_timing_point_button_pressed();
    void on_delete_timing_point_button_pressed();
    void on_timing_point_row_pressed(int index);
    void on_timing_points_updated();
    void on_current_time_button_pressed();
};

#endif