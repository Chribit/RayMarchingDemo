// INCLUDE
#include "./synchronisation.hpp"

// DATA
static struct {
    vector<VkSemaphore> gpu_image_available;
    vector<VkSemaphore> gpu_image_rendered;
    vector<VkFence> cpu_image_presented;
    uint64_t timeout = 1 SECOND;
} synchronisation;

// CODE
VkSemaphore* get_gpu_image_available (int swapchain_frame)
{
    return &synchronisation.gpu_image_available[swapchain_frame];
}

vector<VkSemaphore>* get_image_available_semaphores ()
{
    return &synchronisation.gpu_image_available;
}

void set_image_available_semaphores (vector<VkSemaphore> semaphores)
{
    synchronisation.gpu_image_available = semaphores;
}

VkSemaphore* get_gpu_image_rendered (int swapchain_frame)
{
    return &synchronisation.gpu_image_rendered[swapchain_frame];
}

vector<VkSemaphore>* get_image_rendered_semaphores ()
{
    return &synchronisation.gpu_image_rendered;
}

void set_image_rendered_semaphores (vector<VkSemaphore> semaphores)
{
    synchronisation.gpu_image_rendered = semaphores;
}

VkFence* get_cpu_image_presented (int swapchain_frame)
{
    return &synchronisation.cpu_image_presented[swapchain_frame];
}

vector<VkFence>* get_image_presented_fences ()
{
    return &synchronisation.cpu_image_presented;
}

void set_image_presented_fences (vector<VkFence> fences)
{
    synchronisation.cpu_image_presented = fences;
}

uint64_t get_synchronisation_timeout ()
{
    return synchronisation.timeout;
}

void set_synchronisation_timeout (uint64_t timeout)
{
    synchronisation.timeout = timeout;
}
