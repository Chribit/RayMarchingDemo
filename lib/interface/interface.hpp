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


// NAMESPACE
using namespace std;
using glm::vec2;
using glm::ivec2;
using glm::uvec2;
using glm::bvec2;
using glm::dvec2;
using glm::vec3;
using glm::ivec3;
using glm::uvec3;
using glm::bvec3;
using glm::dvec3;
using glm::vec4;
using glm::ivec4;
using glm::uvec4;
using glm::bvec4;
using glm::dvec4;
using glm::mat2x2;
using glm::degrees;
using glm::radians;
using glm::sign;
using glm::length;


// DEFINE
#pragma once
#define OBJECT object*
#define OBJECT_INSTANCE uint32_t*
#define SHAPE _SHAPE
#define COMPONENT _COMPONENT
#define LINE _LINE


// DEFINE GRAPHICS STRUCTS
#ifndef DEFINED_AABB
#define DEFINED_AABB
struct AABB
{
    vec2 lower_bound;
    vec2 upper_bound;
};
#endif

#ifndef DEFINED_IAABB
#define DEFINED_IAABB
struct indexed_aabb
{
    uint32_t index;
    AABB aabb;
};
#endif

#ifndef DEFINED_BI
#define DEFINED_BI
struct bounding_interval
{
    alignas(4) uint32_t link;
    alignas(4) uint32_t target;
    alignas(4) float coordinate;
};
#endif

#ifndef DEFINED_COL
#define DEFINED_COL
struct colour {
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint32_t alpha = 255;
};
#endif

#ifndef DEFINED_O
#define DEFINED_O
struct object
{
    int index;
    AABB aabb;
    uvec2 primitive_range;
    vector<indexed_aabb> primitive_aabbs;
    uvec2 bih_range;
    vector<bounding_interval> bih;
};
#endif

#ifndef DEFINED_ES
#define DEFINED_ES
struct primitive;
struct instance;
struct engine_state
{
    vector<instance> instances;
    vector<OBJECT_INSTANCE> instance_pointers;
    vector<OBJECT> objects;
    vector<primitive> primitives;
    vector<indexed_aabb> instance_aabbs;
    vector<bounding_interval> tlbih;
};
#endif

const char* get_clipboard ();
void set_clipboard (const char* string);
void set_camera_state (glm::vec2 position, float zoom);
void set_engine_state (engine_state state);
engine_state get_engine_state ();


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
struct SHAPE_DATA
{
    vec2 position_data;
    vec2 dimension_data;
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
        _COMPONENT& react (string component_id, string state_name, function<void(event*)> new_callback);
        _COMPONENT& adopt (_COMPONENT* new_child);
        _COMPONENT& listen (string registered_event, function<void(event*)> new_callback);
        _COMPONENT& ignore (string registered_event, int callback_index);
        _COMPONENT& hover (vector<colour> hover_colours);
        void stop_hover ();
        vector<_COMPONENT*>* get_children ();
        vector<_SHAPE*>* get_shapes ();
        vector<OBJECT_INSTANCE>* get_shape_instances ();
        vector<SPATIAL_STATE>* get_shape_states ();
        string id ();
        void print ();
        void delete_component (bool delete_shapes = false);

    private:
        _COMPONENT* address = nullptr;
        string component_id;

        _COMPONENT* parent;
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
        string id ();
        void print ();
        void delete_line ();

    private:
        _LINE* address = nullptr;
        _COMPONENT* line_component = nullptr;
};
enum alignment
{
    LEFT,
    CENTER,
    RIGHT
};


// CODE
void create_interface (string application_name, uvec2 window_size, function<void()> interface_code);
_COMPONENT& text (string new_component_id, u16string characters, colour font_colour);
void replace_text (string component_id, u16string new_text, colour new_colour);
void initialise_font (colour font_colour);
void set_character (u16string id, _SHAPE* character);
bool has_character (u16string id);
_SHAPE* get_character (u16string character, colour font_colour);
_SHAPE& shape (string id);
_SHAPE& get_shape (string id);
map<string, _SHAPE*>* get_shapes ();
_COMPONENT& get_root ();
_COMPONENT& component (string id);
_COMPONENT& get_component (string id);
map<string, _COMPONENT*>* get_components ();
_LINE& line (string id);
_LINE& get_line (string id);
map<string, _LINE*>* get_lines ();
uint32_t get_instance_count ();
void broadcast_event (_COMPONENT& component, string registered_event, map<string, any> data);
bool is_over_nothing ();
OBJECT_INSTANCE get_front_object ();
OBJECT_INSTANCE get_object_behind_object (OBJECT_INSTANCE query_object);
_COMPONENT& lookup_component (OBJECT_INSTANCE instance_pointer);
vector<string> get_registered_events ();
void register_event (string new_event);
bool is_event_registered (string event);
void deregister_event (string event);
void set_focused (_COMPONENT* current_component);
_COMPONENT* get_focused ();
void force_focused (_COMPONENT* focused_component);
bool get_bool (event* event_data, string key);
bool get_bool (string component_id, string key);
int get_int (event* event_data, string key);
int get_int (string component_id, string key);
float get_float (event* event_data, string key);
float get_float (string component_id, string key);
double get_double (event* event_data, string key);
double get_double (string component_id, string key);
uint32_t get_uint32 (event* event_data, string key);
uint32_t get_uint32 (string component_id, string key);
uint32_t* get_uint32p (event* event_data, string key);
uint32_t* get_uint32p (string component_id, string key);
string get_string (event* event_data, string key);
string get_string (string component_id, string key);
u16string get_u16string (event* event_data, string key);
u16string get_u16string (string component_id, string key);
vec2 get_vec2 (event* event_data, string key);
vec2 get_vec2 (string component_id, string key);
ivec2 get_ivec2 (event* event_data, string key);
ivec2 get_ivec2 (string component_id, string key);
uvec2 get_uvec2 (event* event_data, string key);
uvec2 get_uvec2 (string component_id, string key);
dvec2 get_dvec2 (event* event_data, string key);
dvec2 get_dvec2 (string component_id, string key);
bvec2 get_bvec2 (event* event_data, string key);
bvec2 get_bvec2 (string component_id, string key);
vec3 get_vec3 (event* event_data, string key);
vec3 get_vec3 (string component_id, string key);
ivec3 get_ivec3 (event* event_data, string key);
ivec3 get_ivec3 (string component_id, string key);
uvec3 get_uvec3 (event* event_data, string key);
uvec3 get_uvec3 (string component_id, string key);
dvec3 get_dvec3 (event* event_data, string key);
dvec3 get_dvec3 (string component_id, string key);
bvec3 get_bvec3 (event* event_data, string key);
bvec3 get_bvec3 (string component_id, string key);
vec4 get_vec4 (event* event_data, string key);
vec4 get_vec4 (string component_id, string key);
ivec4 get_ivec4 (event* event_data, string key);
ivec4 get_ivec4 (string component_id, string key);
uvec4 get_uvec4 (event* event_data, string key);
uvec4 get_uvec4 (string component_id, string key);
dvec4 get_dvec4 (event* event_data, string key);
dvec4 get_dvec4 (string component_id, string key);
bvec4 get_bvec4 (event* event_data, string key);
bvec4 get_bvec4 (string component_id, string key);
