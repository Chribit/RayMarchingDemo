// INCLUDE
#include "../command/command.hpp"

// DATA
#ifndef DEFINED_HCA
#define DEFINED_HCA
struct hardware_candidate_attributes {
    uint32_t maximum_shared_memory_size;
    uint32_t* maximum_work_group_count;
    uint32_t* maximum_work_group_size;
    uint32_t maximum_uniform_buffer_range;
    uint32_t maximum_storage_buffer_range;
    uint32_t maximum_push_constant_size;
    uint64_t minimum_uniform_buffer_offset_alignment;
    uint64_t minimum_storage_buffer_offset_alignment;
    float score;
    uint32_t graphics_queue;
    uint32_t presentation_queue;
    uint32_t compute_queue;
};
#endif

// CODE
void initialize_hardware ();
void initialize_hardware_device ();
float determine_hardware_suitability (VkPhysicalDevice gpu_candidate);
bool are_hardware_extensions_supported (VkPhysicalDevice gpu_candidate);
void cleanup_hardware ();

// ACCESS
VkDevice* get_hardware ();
void set_hardware (VkDevice link);
VkPhysicalDevice* get_hardware_device ();
void set_hardware_device (VkPhysicalDevice device);
vector<const char*>* get_hardware_extensions ();
void set_hardware_extensions (vector<const char*> extensions);
VkPhysicalDeviceFeatures* get_hardware_features ();
void set_hardware_features (VkPhysicalDeviceFeatures features);
uint32_t get_hardware_attribute_index ();
void set_hardware_attribute_index (uint32_t index);
vector<hardware_candidate_attributes>* get_hardware_attributes ();
void set_hardware_attributes (vector<hardware_candidate_attributes> attributes);
