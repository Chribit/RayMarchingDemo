// DEFINE
#pragma once
#define SHAPE _SHAPE


// INCLUDE
#include "../compact/compact.hpp"


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
    bool multi_shape;
};

class _SHAPE
{
    public:
        _SHAPE (string id);
        ~_SHAPE ();
        void print ();
        string id ();
        void delete_shape (bool erase = true);
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

        colour colour_data;
};


// CODE
void set_default_shape_colour (colour shape_colour);
colour get_default_shape_colour ();
_SHAPE& shape (string id);
int build_shape_primitive (_SHAPE* target_shape, operation operation_type);
vector<int> build_shape (_SHAPE* target_shape);
