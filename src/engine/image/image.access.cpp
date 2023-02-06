// INCLUDE
#include "./image.hpp"

// DATA
static struct {
    vector<VkImage> swapchain_images;
    vector<VkSampler> swapchain_image_samplers;
    vector<VkImageView> swapchain_image_views;
    VkFormat swapchain_image_format;
    VkColorSpaceKHR swapchain_image_colour_space;
    VkPresentModeKHR swapchain_image_presentation_mode;
    VkExtent2D swapchain_image_extent;
} image;

// CODE
vector<VkImage>* get_swapchain_images ()
{
    return &image.swapchain_images;
}

void set_swapchain_images (vector<VkImage> swapchain_images)
{
    image.swapchain_images = swapchain_images;
}

vector<VkSampler>* get_swapchain_image_samplers ()
{
    return &image.swapchain_image_samplers;
}

void set_swapchain_image_samplers (vector<VkSampler> swapchain_image_samplers)
{
    image.swapchain_image_samplers = swapchain_image_samplers;
}

vector<VkImageView>* get_swapchain_image_views ()
{
    return &image.swapchain_image_views;
}

void set_swapchain_image_views (vector<VkImageView> swapchain_image_views)
{
    image.swapchain_image_views = swapchain_image_views;
}

VkFormat* get_swapchain_image_format ()
{
    return &image.swapchain_image_format;
}

void set_swapchain_image_format (VkFormat format)
{
    image.swapchain_image_format = format;
}

VkColorSpaceKHR* get_swapchain_image_colour_space ()
{
    return &image.swapchain_image_colour_space;
}

void set_swapchain_image_colour_space (VkColorSpaceKHR colour_space)
{
    image.swapchain_image_colour_space = colour_space;
}

VkPresentModeKHR* get_swapchain_image_presentation_mode ()
{
    return &image.swapchain_image_presentation_mode;
}

void set_swapchain_image_presentation_mode (VkPresentModeKHR presentation_mode)
{
    image.swapchain_image_presentation_mode = presentation_mode;
}

VkExtent2D* get_swapchain_image_extent ()
{
    return &image.swapchain_image_extent;
}

void set_swapchain_image_extent (VkExtent2D extent)
{
    image.swapchain_image_extent = extent;
}
