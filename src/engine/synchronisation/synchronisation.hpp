// DEFINE
#define SECOND * 1000000000

// INCLUDE
#include "../command/command.hpp"
#include "../hardware/hardware.hpp"
#include "../structure/structure.hpp"

// CODE
void initialize_synchronisation ();
void await_render_completion (int swapchain_frame);
void reset_fence (int swapchain_frame);
void cleanup_synchronisation ();

// ACCESS
VkSemaphore* get_gpu_image_available (int swapchain_frame);
vector<VkSemaphore>* get_image_available_semaphores ();
void set_image_available_semaphores (vector<VkSemaphore> semaphores);
VkSemaphore* get_gpu_image_rendered (int swapchain_frame);
vector<VkSemaphore>* get_image_rendered_semaphores ();
void set_image_rendered_semaphores (vector<VkSemaphore> semaphores);
VkFence* get_cpu_image_presented (int swapchain_frame);
vector<VkFence>* get_image_presented_fences ();
void set_image_presented_fences (vector<VkFence> fences);
uint64_t get_synchronisation_timeout ();
void set_synchronisation_timeout (uint64_t timeout);
