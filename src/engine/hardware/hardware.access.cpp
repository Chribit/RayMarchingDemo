// INCLUDE
#include "./hardware.hpp"

// DATA
static struct {
    VkDevice link;
    VkPhysicalDevice hardware_device;
    uint32_t hardware_attribute_index;

    #if FOR_MAC
        vector<const char*> hardware_extensions = {
            (char*) VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            (char*) "VK_KHR_portability_subset"
        };
    #else
        vector<const char*> hardware_extensions = {
            (char*) VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    #endif

    VkPhysicalDeviceFeatures hardware_features;
    vector<hardware_candidate_attributes> hardware_attributes;
} hardware;

// CODE
VkDevice* get_hardware ()
{
    return &hardware.link;
}

void set_hardware (VkDevice link)
{
    hardware.link = link;
}

VkPhysicalDevice* get_hardware_device ()
{
    return &hardware.hardware_device;
}

void set_hardware_device (VkPhysicalDevice device)
{
    hardware.hardware_device = device;
}

vector<const char*>* get_hardware_extensions ()
{
    return &hardware.hardware_extensions;
}

void set_hardware_extensions (vector<const char*> extensions)
{
    hardware.hardware_extensions = extensions;
}

VkPhysicalDeviceFeatures* get_hardware_features ()
{
    return &hardware.hardware_features;
}

void set_hardware_features (VkPhysicalDeviceFeatures features)
{
    hardware.hardware_features = features;
}

uint32_t get_hardware_attribute_index ()
{
    return hardware.hardware_attribute_index;
}

void set_hardware_attribute_index (uint32_t index)
{
    hardware.hardware_attribute_index = index;
}

vector<hardware_candidate_attributes>* get_hardware_attributes ()
{
    return &hardware.hardware_attributes;
}

void set_hardware_attributes (vector<hardware_candidate_attributes> attributes)
{
    hardware.hardware_attributes = attributes;
}

