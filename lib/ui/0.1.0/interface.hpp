// DEFINE
#pragma once
#define SHAPE static _SHAPE
#define COMPONENT static _COMPONENT
#define LINE static _LINE


// INCLUDE
#include "cstring"
#include "iostream"
#include "vector"
#include "algorithm"
#include "functional"
#include "numeric"
#include "glm/glm.hpp"
#include "map"
#include "any"


// GRAPHICS INCLUDE
#ifndef DEFINED_G
#define DEFINED_G
#include "../../../../Graphics/release/0.1.3/graphics.hpp"
#endif


// NAMESPACE
using namespace std;
using glm::vec2;
using glm::dvec2;
using glm::mat2x2;
using glm::degrees;
using glm::radians;
using glm::sign;
using glm::length;


// DATA
struct SHAPE_DATA
{
    glm::vec2 position_data;
    glm::vec2 dimension_data;
    float radius_data;
    float thickness_data;
    float rotation_data;
    uint16_t x_repetition;
    uint16_t y_repetition;
    colour colour_data;
};
class _SHAPE
{
    public:
        _SHAPE (string id);
        ~_SHAPE ();
        void print ();
        string id ();
        void delete_shape ();
        _SHAPE& x (float new_x);
        _SHAPE& y (float new_y);
        _SHAPE& width (float new_width);
        _SHAPE& height (float new_height);
        _SHAPE& radius (float new_radius);
        _SHAPE& round (float new_radius);
        _SHAPE& thickness (float new_thickness);
        _SHAPE& rotate (float new_rotation);
        _SHAPE& scale (float new_scale);
        _SHAPE& add (_SHAPE* joined_shape);
        _SHAPE& repeat (uint16_t x_repetition, uint16_t y_repetition);
        _SHAPE& subtract (_SHAPE* operator_shape);
        _SHAPE& intersect (_SHAPE* operator_shape);
        _SHAPE& red (uint32_t new_red);
        _SHAPE& green (uint32_t new_green);
        _SHAPE& blue (uint32_t new_blue);
        _SHAPE& visible (bool is_visible);
        SHAPE_DATA data ();
        vector<_SHAPE*>* additions ();
        vector<_SHAPE*>* subtractions ();
        vector<_SHAPE*>* intersections ();
        _SHAPE& build ();
        bool built ();
        OBJECT get_build ();

    private:
        _SHAPE* address = nullptr;
        OBJECT shape_object = nullptr;
        vector<_SHAPE*> joined_shapes = {};
        bool is_built = false;
        string shape_id;

        glm::vec2 position_data = {0.0, 0.0};
        glm::vec2 dimension_data = {0.0, 0.0};
        float radius_data = 0.0;
        float thickness_data = 0.0;

        float rotation_data = 0.0;
        vector<_SHAPE*> intersection_operators = {};
        vector<_SHAPE*> subtraction_operators = {};
        uint16_t x_repetition = 0;
        uint16_t y_repetition = 0;

        colour colour_data = {0, 0, 0, 255};
};
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
class _LINE
{
    public:
        _LINE (string id);
        ~_LINE ();
        _COMPONENT& get_component ();
        vec2 from ();
        _LINE& from (vec2 start_point);
        vec2 to ();
        _LINE& to (vec2 end_point);
        float radius ();
        _LINE& radius (float new_radius);
        float thickness ();
        _LINE& thickness (float new_thickness);
        uint32_t red ();
        _LINE& red (uint32_t new_red);
        uint32_t green ();
        _LINE& green (uint32_t new_green);
        uint32_t blue ();
        _LINE& blue (uint32_t new_blue);
        _LINE& draw ();

    private:
        _COMPONENT* line_component = nullptr;
};


// CODE
void create_interface (string application_name, function<void()> code);
_SHAPE& shape (string id);
_SHAPE& get_shape (string id);
_COMPONENT* get_root ();
_COMPONENT& component (string id);
_COMPONENT& get_component (string id);
_LINE& line (string id);
_LINE& get_line (string id);
uint32_t get_instance_count ();