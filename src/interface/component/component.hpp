// DEFINE
#pragma once
#define COMPONENT _COMPONENT


// INCLUDE
#include "../compact/compact.hpp"
#include "../shape/shape.hpp"


// DATA
class event;

struct SPATIAL_STATE {
    vec2 position;
    float rotation;
    float scale;
    uint32_t depth;
};

class _COMPONENT
{
    public:
        _COMPONENT ();
        _COMPONENT (string id);
        ~_COMPONENT ();
        _COMPONENT& add (_SHAPE* new_shape);
        _COMPONENT& replace (int shape_index, _SHAPE* new_shape);
        vec2 position ();
        vec2 position (int shape_index);
        _COMPONENT& position (vec2 coordinates);
        _COMPONENT& position (int shape_index, vec2 coordinates);
        _COMPONENT& x (float x_coordinate);
        _COMPONENT& x (int shape_index, float x_coordinate);
        _COMPONENT& y (float y_coordinate);
        _COMPONENT& y (int shape_index, float y_coordinate);
        float scale ();
        float scale (int shape_index);
        _COMPONENT& scale (float scale);
        _COMPONENT& scale (int shape_index, float scale);
        float rotation ();
        float rotation (int shape_index);
        _COMPONENT& rotation (float angle);
        _COMPONENT& rotation (int shape_index, float angle);
        _COMPONENT& front ();
        _COMPONENT& state (string key, any value);
        any state (string key);
        _COMPONENT& react (string component_id, string state_name, function<void(event*)> new_callback);
        _COMPONENT& adopt (_COMPONENT* new_child);
        _COMPONENT& expel (_COMPONENT* child);
        _COMPONENT& listen (string registered_event, function<void(event*)> new_callback);
        _COMPONENT& ignore (string registered_event, int callback_index);
        _COMPONENT& hover (vector<colour> hover_colours);
        void stop_hover ();
        bool get_hovering ();
        SPATIAL_STATE* get_component_spatial_state ();
        void add_total_shape_count (uint32_t summand);
        void add_total_child_count (uint32_t summand);
        _COMPONENT* get_parent ();
        vector<_COMPONENT*>* get_children ();
        vector<_SHAPE*>* get_shapes ();
        vector<OBJECT_INSTANCE>* get_shape_instances ();
        vector<SPATIAL_STATE>* get_shape_states ();
        void run_callbacks (string registered_event, event* event_data);
        void set_state_reactor (string state_name, string reactor_id);
        map<string, vector<function<void(event*)>>>* get_component_event_callbacks ();
        string id ();
        void print ();
        void delete_component (bool delete_shapes = false);

    private:
        _COMPONENT* address = nullptr;
        string component_id;
        bool has_hovering = false;
        uvec2 hovering_callbacks = vec2(0, 0);

        _COMPONENT* parent = nullptr;
        uint32_t parent_child_index;
        vector<_COMPONENT*> children;
        uint32_t total_child_count = 0;
        uint32_t total_shape_count = 0;

        SPATIAL_STATE component_spatial_state = {{0.0, 0.0}, 0.0, 1.0, 0};
        map<string, any> component_state;
        map<string, vector<function<void(event*)>>> component_event_callbacks;
        map<string, vector<string>> component_state_reactors;

        vector<_SHAPE*> shapes;
        vector<OBJECT_INSTANCE> shape_instances;
        vector<SPATIAL_STATE> shape_states;
};


// CODE
_COMPONENT& component (string id);
vec2 rotate (vec2 point, float degrees);
