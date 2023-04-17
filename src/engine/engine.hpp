// INCLUDE
#include "./compact/compact.hpp"
#include "./file/file.hpp"
#include "./window/window.hpp"
#include "./validation/validation.hpp"
#include "./vulkan/vulkan.hpp"
#include "./hardware/hardware.hpp"
#include "./shading/shading.hpp"
#include "./swapchain/swapchain.hpp"
#include "./image/image.hpp"
#include "./synchronisation/synchronisation.hpp"
#include "./memory/memory.hpp"
#include "./descriptor/descriptor.hpp"
#include "./instance/instance.hpp"
#include "./object/object.hpp"
#include "./primitive/primitive.hpp"
#include "./colour/colour.hpp"
#include "./bih/bih.hpp"
#include "./traversal/traversal.hpp"
#include "./event/event.hpp"


// DATA
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
void start_engine (string engine_name, string application_name, uint32_t width, uint32_t height);
void initialize ();
void run ();
void render ();
void set_camera_state (glm::vec2 position, float zoom);
void cleanup ();


// ACCESS
string get_engine_name ();
void set_engine_name (string name);
string get_application_name ();
void set_application_name (string name);
int get_swapchain_frame ();
void set_swapchain_frame (int swapchain_frame);
uint64_t get_frame_time ();
void set_frame_time (uint64_t frame_time);
bool get_initialising ();
void set_initialising (bool initialising);
void set_engine_state (engine_state state);
engine_state get_engine_state ();
glm::vec2 get_mouse_coordinates ();
void set_pixel_objects (vector<OBJECT_INSTANCE> pixel_objects);
vector<OBJECT_INSTANCE> get_pixel_objects ();
