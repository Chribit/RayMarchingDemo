// DEFINE
#pragma once
#if FOR_WIN
    #define PATH_SEPERATOR "\\"
#else
    #define PATH_SEPERATOR "/"
#endif
#define GLFW_INCLUDE_VULKAN
#define OBJECT_INSTANCE uint32_t*
#define OBJECT object*


// INCLUDE
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "cstring"
#include "chrono"
#include "iostream"
#include "vector"
#include "fstream"
#include "set"
#include "optional"
#include "cstdint"
#include "limits"
#include "algorithm"
#include "math.h"
#include "iomanip"
#include "functional"
#include "numeric"
#if FOR_MAC
    #include "mach-o/dyld.h"
#elif FOR_WIN
    #include "libloaderapi.h"
#elif FOR_LIN
    #include "unistd.h"
#endif


// NAMESPACE
using namespace std;
using namespace std::chrono;


// DATA
#ifndef DEFINED_FI
#define DEFINED_FI
struct folder_item
{
    bool is_folder;
    string name;
    string path;
    uint32_t size = 0;
};
#endif

#ifndef DEFINED_AABB
#define DEFINED_AABB
struct AABB
{
    glm::vec2 lower_bound;
    glm::vec2 upper_bound;
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

#ifndef DEFINED_I
#define DEFINED_I
struct instance
{
    alignas(8) glm::vec2 position;
    alignas(4) float scale;
    alignas(4) float rotation;
    alignas(4) uint32_t data_one;
    alignas(4) uint32_t data_two;
};
#endif

#ifndef DEFINED_O
#define DEFINED_O
struct object
{
    int index;
    AABB aabb;
    glm::uvec2 primitive_range;
    vector<indexed_aabb> primitive_aabbs;
    glm::uvec2 bih_range;
    vector<bounding_interval> bih;
};
#endif

#ifndef DEFINED_P
#define DEFINED_P
struct primitive
{
    alignas(8) glm::vec2 position;
    alignas(8) glm::vec2 dimensions;
    alignas(4) float radius;
    alignas(4) float thickness;
    alignas(4) uint32_t x_parameter;
    alignas(4) uint32_t y_parameter;
    alignas(4) float rotation;
    alignas(4) uint32_t colour;
};
#endif

#ifndef DEFINED_PCON
#define DEFINED_PCON
struct primitive_constructor
{
    glm::vec2 position = {0.0, 0.0};
    glm::vec2 dimensions = {0.0, 0.0};
    float radius = 0.0;
    float thickness = 0.0;
};
#endif

#ifndef DEFINED_OP
#define DEFINED_OP
enum operation
{
    REPETITION =   0b00000000000000000000000000000000,
    INTERSECTION = 0b10000000000000000000000000000000,
    DIFFERENCE =   0b11000000000000000000000000000000
};
#endif

#ifndef DEFINED_OPCON
#define DEFINED_OPCON
struct operation_constructor
{
    operation type;
    uint32_t target = 0;
    uint32_t x_repeat = 0;
    uint16_t y_repeat = 0;
    float rotation = 0.0;
};
#endif

#ifndef DEFINED_ES
#define DEFINED_ES
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


// CODE
string get_executable_path ();
string get_executable_folder_path ();
string get_absolute_path (string relative_path);
bool path_exists (string path);
void create_folder (string path);
vector<folder_item> read_folder (string path);
string read_file (string path);
void write_file (string path, string bytes, uint32_t byte_offset = 0);
void delete_path (string path);
void start_engine (string engine_name, string application_name, uint32_t width, uint32_t height);
void set_camera_state (glm::vec2 position, float zoom);
void set_engine_state (engine_state state);
engine_state get_engine_state ();
void set_pixel_objects (vector<OBJECT_INSTANCE> pixel_objects);
vector<OBJECT_INSTANCE> get_pixel_objects ();
GLFWwindow* get_window ();
glm::vec2 get_mouse_coordinates ();
void on_initialisation (function<void()> callback);
void on_render_completion (function<void()> callback);
void on_mouse_move (function<void(glm::vec2)> callback);
void on_mouse_down (function<void(glm::vec2)> callback);
void on_mouse_up (function<void(glm::vec2)> callback);
void on_mouse_scroll (function<void(glm::vec2, glm::dvec2)> callback);
void on_mouse_enter (function<void(glm::vec2)> callback);
void on_mouse_leave (function<void(glm::vec2)> callback);
void on_window_resize (function<void(glm::uvec2)> callback);
void on_key_down (function<void(int, int, int)> callback);
void on_key_up (function<void(int, int, int)> callback);
const char* get_clipboard ();
void set_clipboard (const char* string);
OBJECT_INSTANCE create_instance (void* target_object, glm::vec2 position, float scale, float rotation);
uint16_t get_instance_object_index (OBJECT_INSTANCE target_instance);
uint32_t get_instance_depth (OBJECT_INSTANCE target_instance);
void set_instance_depth (OBJECT_INSTANCE target_instance, uint32_t new_depth);
void update_instance_object (OBJECT_INSTANCE target_instance, void* new_object);
void update_instance_position (OBJECT_INSTANCE target_instance, glm::vec2 new_position);
void update_instance_scale (OBJECT_INSTANCE target_instance, float new_scale);
void update_instance_rotation (OBJECT_INSTANCE target_instance, float new_rotation);
void update_instance_depth (OBJECT_INSTANCE target_instance, uint32_t new_depth);
void delete_instances (vector<OBJECT_INSTANCE> target_instance);
void delete_object_instances (void* target_object);
OBJECT create_object (vector< vector<int> > primitive_csg_hierarchies);
void delete_object (OBJECT target_object, bool cleanup = false);
int create_primitive (primitive_constructor primitive_data, operation_constructor operation_data, colour colour_data);
float get_primitive_signed_distance (glm::vec2 point, int primitive_index);
void update_primitive_colour (void* target_object, int primitive_index, colour colour_data);
void delete_object_primitives (void* target_object);