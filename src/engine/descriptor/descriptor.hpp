// INCLUDE
#include "../hardware/hardware.hpp"
#include "../memory/memory.hpp"
#include "../swapchain/swapchain.hpp"

// DATA
#ifndef DEFINED_SSB
#define DEFINED_SSB
struct shader_stage_bits {
    bool vertex = false;
    bool tessellation_control = false;
    bool tessellation_evaluation = false;
    bool geometry = false;
    bool fragment = false;
    bool compute = false;
};
#endif

// CODE
void initialize_descriptor ();
void initialize_descriptor_layout ();
VkDescriptorType determine_descriptor_type (string name);
void initialize_descriptor_pools ();
void initialize_descriptor_set ();
void cleanup_descriptor ();

// ACCESS
vector<VkDescriptorSet> get_descriptor_sets (int swapchain_frame);
VkDescriptorSet* get_descriptor_buffer_set ();
void set_descriptor_buffer_set (VkDescriptorSet set);
vector<VkDescriptorSet>* get_descriptor_image_sets ();
void set_descriptor_image_sets (vector<VkDescriptorSet> set);
VkDescriptorSetLayout* get_descriptor_buffer_layout ();
void set_descriptor_buffer_layout (VkDescriptorSetLayout layout);
VkDescriptorSetLayout* get_descriptor_image_layout ();
void set_descriptor_image_layout (VkDescriptorSetLayout layout);
VkDescriptorPool* get_descriptor_buffer_pool ();
void set_descriptor_buffer_pool (VkDescriptorPool pool);
VkDescriptorPoolSize* get_descriptor_buffer_pool_size ();
void set_descriptor_buffer_pool_size (VkDescriptorPoolSize count);
VkDescriptorPool* get_descriptor_image_pool ();
void set_descriptor_image_pool (VkDescriptorPool pool);
VkDescriptorPoolSize* get_descriptor_image_pool_size ();
void set_descriptor_image_pool_size (VkDescriptorPoolSize size);
