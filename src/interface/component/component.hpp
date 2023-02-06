// DEFINE
#pragma once
#define COMPONENT static _COMPONENT


// INCLUDE
#include "../compact/compact.hpp"
#include "../shape/shape.hpp"


// DATA
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
        _COMPONENT& adopt (_COMPONENT* new_child);
        _COMPONENT& expel (int child_index);
        string id ();
        void print ();
        void delete_component ();

    private:
        _COMPONENT* address = nullptr;
        string component_id;

        _COMPONENT* parent;
        vector<_COMPONENT*> children;
        uint32_t total_shape_count = 0;

        SPATIAL_STATE component_spatial_state = {{0.0, 0.0}, 0.0, 1.0, 0};
        map<string, any> component_state;

        vector<_SHAPE*> shapes;
        vector<OBJECT_INSTANCE> shape_instances;
        vector<SPATIAL_STATE> shape_states;
};


// CODE
_COMPONENT& component (string id);
vec2 rotate (vec2 point, float degrees);
