// INCLUDE
#include "./memory.hpp"

// DATA
static struct {
    uint64_t buffer_available = 0;
    uint32_t buffer_type_index;

    push_constants push_constants;
    VkPushConstantRange push_constant_range;

    VkDeviceMemory buffer_memory;

    VkBuffer instance_buffer;
    uint64_t instance_buffer_size = 1;

    VkBuffer primitive_buffer;
    uint64_t primitive_buffer_size = 1;

    VkBuffer blbihs_buffer;
    uint64_t blbihs_buffer_size = 1;

    VkBuffer tlbih_buffer;
    uint64_t tlbihs_buffer_size = 1;
} memory;


// CODE
void set_buffer_available_memory (uint64_t available_memory)
{
    memory.buffer_available = available_memory;
}

uint64_t get_buffer_available_memory ()
{
    return memory.buffer_available;
}

void set_buffer_memory_type_index (uint32_t memory_type_index)
{
    memory.buffer_type_index = memory_type_index;
}

uint32_t get_buffer_memory_type_index ()
{
    return memory.buffer_type_index;
}

void set_buffer_memory (VkDeviceMemory device_memory)
{
    memory.buffer_memory = device_memory;
}

VkDeviceMemory* get_buffer_memory ()
{
    return &memory.buffer_memory;
}


// PUSH CONSTANTS
void set_push_constants (push_constants constants)
{
    memory.push_constants = constants;
}

push_constants* get_push_constants ()
{
    return &memory.push_constants;
}

void set_push_constant_range (VkPushConstantRange range)
{
    memory.push_constant_range = range;
}

VkPushConstantRange* get_push_constant_range ()
{
    return &memory.push_constant_range;
}

void set_push_constant_window_size (glm::ivec2 window_size)
{
    memory.push_constants.window_size = window_size;
}

glm::ivec2 get_push_constant_window_size ()
{
    return memory.push_constants.window_size;
}

void set_push_constant_scene_aabb (AABB scene_aabb)
{
    memory.push_constants.scene_lower_bound = scene_aabb.lower_bound;
    memory.push_constants.scene_upper_bound = scene_aabb.upper_bound;
}

AABB get_push_constant_scene_aabb ()
{
    return {memory.push_constants.scene_lower_bound, memory.push_constants.scene_upper_bound};
}

void set_push_constant_primitive_count (uint32_t primitive_count)
{
    memory.push_constants.primitive_count = primitive_count;
}

void set_push_constant_camera_position (glm::vec2 camera_position)
{
    memory.push_constants.camera_position = camera_position;
}

glm::vec2 get_push_constant_camera_position ()
{
    return memory.push_constants.camera_position;
}

void set_push_constant_camera_zoom (float camera_zoom)
{
    memory.push_constants.camera_zoom = camera_zoom;
}

float get_push_constant_camera_zoom ()
{
    return memory.push_constants.camera_zoom;
}


// INSTANCE BUFFER
void set_instance_buffer (VkBuffer buffer)
{
    memory.instance_buffer = buffer;
}

VkBuffer* get_instance_buffer ()
{
    return &memory.instance_buffer;
}

void set_instance_buffer_size (uint64_t buffer_size)
{
    memory.instance_buffer_size = buffer_size;
}

uint64_t get_instance_buffer_size ()
{
    return memory.instance_buffer_size;
}


// PRIMITIVE BUFFER
void set_primitive_buffer (VkBuffer buffer)
{
    memory.primitive_buffer = buffer;
}

VkBuffer* get_primitive_buffer ()
{
    return &memory.primitive_buffer;
}

void set_primitive_buffer_size (uint64_t buffer_size)
{
    memory.primitive_buffer_size = buffer_size;
}

uint64_t get_primitive_buffer_size ()
{
    return memory.primitive_buffer_size;
}


// BOTTOM LEVEL BOUNDING INTERVAL BUFFER
void set_blbihs_buffer (VkBuffer buffer)
{
    memory.blbihs_buffer = buffer;
}

VkBuffer* get_blbihs_buffer ()
{
    return &memory.blbihs_buffer;
}

void set_blbihs_buffer_size (uint64_t buffer_size)
{
    memory.blbihs_buffer_size = buffer_size;
}

uint64_t get_blbihs_buffer_size ()
{
    return memory.blbihs_buffer_size;
}


// TOP LEVEL BOUNDING INTERVAL BUFFER
void set_tlbih_buffer (VkBuffer buffer)
{
    memory.tlbih_buffer = buffer;
}

VkBuffer* get_tlbih_buffer ()
{
    return &memory.tlbih_buffer;
}

void set_tlbih_buffer_size (uint64_t buffer_size)
{
    memory.tlbihs_buffer_size = buffer_size;
}

uint64_t get_tlbih_buffer_size ()
{
    return memory.tlbihs_buffer_size;
}
