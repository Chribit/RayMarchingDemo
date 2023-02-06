// INCLUDE
#include "./swapchain.hpp"

// DATA
static struct {
    VkSwapchainKHR instance;
    swapchain_support_details support_details;
    uint32_t image_count;
    uint32_t image_index = 1;
} swapchain;

// CODE
VkSwapchainKHR* get_swapchain ()
{
    return &swapchain.instance;
}

void set_swapchain (VkSwapchainKHR new_swapchain)
{
    swapchain.instance = new_swapchain;
}

swapchain_support_details* get_swapchain_support_details ()
{
    return &swapchain.support_details;
}

void set_swapchain_support_details (swapchain_support_details details)
{
    swapchain.support_details = details;
}

uint32_t* get_swapchain_image_count ()
{
    return &swapchain.image_count;
}

void set_swapchain_image_count (uint32_t image_count)
{
    swapchain.image_count = image_count;
}

uint32_t* get_swapchain_image_index ()
{
    return &swapchain.image_index;
}

void set_swapchain_image_index (uint32_t index)
{
    swapchain.image_index = index;
}
