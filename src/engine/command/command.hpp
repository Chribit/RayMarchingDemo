// INCLUDE
#include "../window/window.hpp"

// DATA
#ifndef DEFINED_QFI
#define DEFINED_QFI
struct queue_family_indices {
    optional<uint32_t> graphics_family;
    optional<uint32_t> presentation_family;
    optional<uint32_t> compute_family;
    optional<uint32_t> transfer_family;
    bool is_complete()
    {
        return graphics_family.has_value() && presentation_family.has_value() && compute_family.has_value();
    }
};
#endif
#ifndef DEFINED_QFB
#define DEFINED_QFB
struct queue_flag_bits {
    bool graphics = false;
    bool compute = false;
    bool transfer = false;
    bool sparse_binding = false;
};
#endif

// CODE
void initialize_command ();
queue_family_indices find_queue_families (VkPhysicalDevice hardware_candidate, bool candidate);
queue_flag_bits get_queue_bits (VkQueueFlags flags);
bool determine_queue_supports_presentation (VkPhysicalDevice hardware, int queue_index);
void create_command_pool ();
void create_command_buffers ();
void record_command_buffer (int swapchain_frame);
void reset_command_buffer (int swapchain_frame);
void submit_command_buffer (int swapchain_frame);
void cleanup_command ();

// ACCESS
VkCommandPool* get_command_pool ();
void set_command_pool (VkCommandPool pool);
vector<VkCommandBuffer>* get_command_buffers ();
void set_command_buffers (vector<VkCommandBuffer> buffers);
VkCommandBuffer* get_command_buffer (int swapchain_frame);
VkQueue* get_graphics_queue ();
void set_graphics_queue (VkQueue queue);
VkQueue* get_presentation_queue ();
void set_presentation_queue (VkQueue queue);
VkQueue* get_compute_queue ();
void set_compute_queue (VkQueue queue);
