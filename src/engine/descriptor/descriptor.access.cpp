// INCLUDE
#include "./descriptor.hpp"

// DATA
static struct {
    VkDescriptorSet buffer_set;
    vector<VkDescriptorSet> image_sets;
    VkDescriptorSetLayout buffer_layout;
    VkDescriptorSetLayout image_layout;
    VkDescriptorPool buffer_pool;
    VkDescriptorPoolSize buffer_pool_size;
    VkDescriptorPool image_pool;
    VkDescriptorPoolSize image_pool_size;
} descriptor;

// CODE
vector<VkDescriptorSet> get_descriptor_sets (int swapchain_frame)
{
    vector<VkDescriptorSet> sets;
    sets.push_back(descriptor.buffer_set);
    sets.push_back(descriptor.image_sets[swapchain_frame]);
    return sets;
}

VkDescriptorSet* get_descriptor_buffer_set ()
{
    return &descriptor.buffer_set;
}

void set_descriptor_buffer_set (VkDescriptorSet set)
{
    descriptor.buffer_set = set;
}

vector<VkDescriptorSet>* get_descriptor_image_sets ()
{
    return &descriptor.image_sets;
}

void set_descriptor_image_sets (vector<VkDescriptorSet> set)
{
    descriptor.image_sets = set;
}

VkDescriptorSetLayout* get_descriptor_buffer_layout ()
{
    return &descriptor.buffer_layout;
}

void set_descriptor_buffer_layout (VkDescriptorSetLayout layout)
{
    descriptor.buffer_layout = layout;
}

VkDescriptorSetLayout* get_descriptor_image_layout ()
{
    return &descriptor.image_layout;
}

void set_descriptor_image_layout (VkDescriptorSetLayout layout)
{
    descriptor.image_layout = layout;
}

VkDescriptorPool* get_descriptor_buffer_pool ()
{
    return &descriptor.buffer_pool;
} 

void set_descriptor_buffer_pool (VkDescriptorPool pool)
{
    descriptor.buffer_pool = pool;
}

VkDescriptorPoolSize* get_descriptor_buffer_pool_size ()
{
    return &descriptor.buffer_pool_size;
}

void set_descriptor_buffer_pool_size (VkDescriptorPoolSize size)
{
    descriptor.buffer_pool_size = size;
}

VkDescriptorPool* get_descriptor_image_pool ()
{
    return &descriptor.image_pool;
} 

void set_descriptor_image_pool (VkDescriptorPool pool)
{
    descriptor.image_pool = pool;
}

VkDescriptorPoolSize* get_descriptor_image_pool_size ()
{
    return &descriptor.image_pool_size;
}

void set_descriptor_image_pool_size (VkDescriptorPoolSize size)
{
    descriptor.image_pool_size = size;
}
