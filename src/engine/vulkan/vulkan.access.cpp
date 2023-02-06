// INCLUDE
#include "./vulkan.hpp"

// DATA
static struct {
    VkInstance instance;
    vector<const char*> extensions;
} vulkan;

// CODE
VkInstance* get_vulkan_instance ()
{
    return &vulkan.instance;
}

void set_vulkan_instance (VkInstance instance)
{
    vulkan.instance = instance;
}

vector<const char*>* get_vulkan_extensions ()
{
    return &vulkan.extensions;
}

void set_vulkan_extensions (vector<const char*> extensions)
{
    vulkan.extensions = extensions;
}

