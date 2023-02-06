// INCLUDE
#include "./command.hpp"

// DATA
static struct {
    VkCommandPool pool;
    vector<VkCommandBuffer> buffers;
    VkQueue graphics_queue;
    VkQueue presentation_queue;
    VkQueue compute_queue;
} command;

// CODE
VkCommandPool* get_command_pool ()
{
    return &command.pool;
}

void set_command_pool (VkCommandPool pool)
{
    command.pool = pool;
}

vector<VkCommandBuffer>* get_command_buffers ()
{
    return &command.buffers;
}

void set_command_buffers (vector<VkCommandBuffer> buffers)
{
    command.buffers = buffers;
}

VkCommandBuffer* get_command_buffer (int swapchain_frame)
{
    return &command.buffers[swapchain_frame];
}

VkQueue* get_graphics_queue ()
{
    return &command.graphics_queue;
}

void set_graphics_queue (VkQueue queue)
{
    command.graphics_queue = queue;
}

VkQueue* get_presentation_queue ()
{
    return &command.presentation_queue;
}

void set_presentation_queue (VkQueue queue)
{
    command.presentation_queue = queue;
}

VkQueue* get_compute_queue ()
{
    return &command.compute_queue;  
}

void set_compute_queue (VkQueue queue)
{
    command.compute_queue = queue;
}

