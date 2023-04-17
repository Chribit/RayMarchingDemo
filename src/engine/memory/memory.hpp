// INCLUDE
#include "../hardware/hardware.hpp"
#include "../structure/structure.hpp"
#include "../image/image.hpp"


// DATA
#ifndef DEFINED_MFB
#define DEFINED_MFB
struct memory_flag_bits {
    bool device_local = false;
    bool host_visible = false;
    bool host_coherent = false;
    bool host_cached = false;
    bool lazily_allocated = false;
};
#endif

#ifndef DEFINED_MHFB
#define DEFINED_MHFB
struct memory_heap_flag_bits {
    bool device_local = false;
    bool multi_instance = false;
};
#endif

#ifndef DEFINED_MBFB
#define DEFINED_MBFB
struct memory_buffer_flag_bits {
    bool transfer_source = false;
    bool transfer_destination = false;
    bool uniform_texel_buffer = false;
    bool storage_texel_buffer = false;
    bool uniform_buffer = false;
    bool storage_buffer = false;
    bool index_buffer = false;
    bool vertex_buffer = false;
    bool indirect_buffer = false;
};
#endif

#ifndef DEFINED_PC
#define DEFINED_PC
struct push_constants {
    alignas(8) glm::ivec2 window_size;
    alignas(8) glm::vec2 scene_lower_bound;
    alignas(8) glm::vec2 scene_upper_bound;
    alignas(8) glm::vec2 camera_position;
    alignas(4) uint32_t primitive_count;
    alignas(4) float camera_zoom;
};
#endif


// CODE
void initialize_memory ();
void initialize_memory_type ();
memory_flag_bits parse_memory_bits (VkMemoryPropertyFlags given_flags);
memory_heap_flag_bits parse_heap_bits (VkMemoryHeapFlags given_flags);
uint64_t determine_memory_size ();
void initialize_push_constants ();
void initialize_buffers ();
void initialize_memory_allocation ();
void bind_buffers ();
uint32_t set_buffer_bits (memory_buffer_flag_bits given_bits);
void transmit_object_data (VkCommandBuffer command_buffer);
void cleanup_memory ();


// ACCESS
void set_buffer_available_memory (uint64_t available_memory);
uint64_t get_buffer_available_memory ();
void set_buffer_memory_type_index (uint32_t memory_type_index);
uint32_t get_buffer_memory_type_index ();
void set_buffer_memory (VkDeviceMemory device_memory);
VkDeviceMemory* get_buffer_memory ();


// ACCESS PUSH CONSTANTS
void set_push_constants (push_constants constants);
push_constants* get_push_constants ();
void set_push_constant_range (VkPushConstantRange range);
VkPushConstantRange* get_push_constant_range ();
void set_push_constant_window_size (glm::ivec2 window_size);
glm::ivec2 get_push_constant_window_size ();
void set_push_constant_scene_aabb (AABB scene_aabb);
AABB get_push_constant_scene_aabb ();
void set_push_constant_primitive_count (uint32_t primitive_count);
void set_push_constant_camera_position (glm::vec2 camera_position);
glm::vec2 get_push_constant_camera_position ();
void set_push_constant_camera_zoom (float camera_zoom);
float get_push_constant_camera_zoom ();


// ACCESS INSTANCE BUFFER
void set_instance_buffer (VkBuffer buffer);
VkBuffer* get_instance_buffer ();
void set_instance_buffer_size (uint64_t buffer_size);
uint64_t get_instance_buffer_size ();


// ACCESS PRIMITIVE BUFFER
void set_primitive_buffer (VkBuffer buffer);
VkBuffer* get_primitive_buffer ();
void set_primitive_buffer_size (uint64_t buffer_size);
uint64_t get_primitive_buffer_size ();


// ACCESS BOTTOM LEVEL INTERVAL BUFFER
void set_blbihs_buffer (VkBuffer buffer);
VkBuffer* get_blbihs_buffer ();
void set_blbihs_buffer_size (uint64_t buffer_size);
uint64_t get_blbihs_buffer_size ();


// ACCESS TOP LEVEL BOUNDING INTERVAL BUFFER
void set_tlbih_buffer (VkBuffer buffer);
VkBuffer* get_tlbih_buffer ();
void set_tlbih_buffer_size (uint64_t buffer_size);
uint64_t get_tlbih_buffer_size ();
