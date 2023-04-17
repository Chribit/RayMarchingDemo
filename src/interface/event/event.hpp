// DEFINE
#pragma once


// INCLUDE
#include "../compact/compact.hpp"
#include "../component/component.hpp"


// DATA
class event
{
    public:
        event ();
        ~event ();
        void set (string key, any value);
        any get (string key);
    
    private:
        map<string, any> event_data;
};


// CODE
void initialise_events ();
void broadcast_event (_COMPONENT& component, string registered_event, map<string, any> data);
bool is_over_nothing ();
OBJECT_INSTANCE get_front_object ();
OBJECT_INSTANCE get_object_behind_object (OBJECT_INSTANCE query_object);


// ACCESS
void register_component (OBJECT_INSTANCE instance_pointer, _COMPONENT* component);
_COMPONENT* lookup_component_pointer (OBJECT_INSTANCE instance_pointer);
_COMPONENT& lookup_component (OBJECT_INSTANCE instance_pointer);
void deregister_component (_COMPONENT* component);
vector<string> get_registered_events ();
void register_event (string new_event);
void register_event (string new_event, bool reaction);
bool is_event_registered (string event);
void deregister_event (string event);
void set_currently_over (_COMPONENT* current_component);
_COMPONENT* get_currently_over ();
void set_last_mouse_down (_COMPONENT* current_component);
_COMPONENT* get_last_mouse_down ();
void set_dragged_component (_COMPONENT *current_component);
_COMPONENT* get_dragged_component ();
void set_dragging (bool status);
bool is_dragging ();
void set_last_clicked (_COMPONENT *current_component);
_COMPONENT *get_last_clicked ();
void set_last_clicked_time (uint64_t time);
uint64_t get_last_clicked_time ();
void set_focused (_COMPONENT* current_component);
_COMPONENT* get_focused ();
void set_last_key_down (_COMPONENT* current_component);
_COMPONENT* get_last_key_down ();
void set_modifier_keys (int key_index, int modifier_flags);
u16string get_key_string (int key_index);
string get_key_name(int key_index);
bool get_shift_pressed ();
bool get_alt_pressed ();
bool get_control_pressed ();
bool get_command_pressed ();
bool get_capslock_active ();
bool get_numlock_active ();
