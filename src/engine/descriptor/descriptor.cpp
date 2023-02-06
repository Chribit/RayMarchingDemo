// INCLUDE
#include "./descriptor.hpp"
#include "../shading/shading.hpp"

// CODE
void initialize_descriptor ()
{
    initialize_descriptor_layout();
    initialize_descriptor_pools();
    initialize_descriptor_set();
}

void initialize_descriptor_layout ()
{
    vector<VkDescriptorSetLayoutBinding> buffer_bindings = {
        { 0, determine_descriptor_type("storage buffer"), 1, set_shader_stage_bits({ .compute = true }), 0 },
        { 1, determine_descriptor_type("storage buffer"), 1, set_shader_stage_bits({ .compute = true }), 0 },
        { 2, determine_descriptor_type("storage buffer"), 1, set_shader_stage_bits({ .compute = true }), 0 },
        { 3, determine_descriptor_type("storage buffer"), 1, set_shader_stage_bits({ .compute = true }), 0 }
    };
    vector<VkDescriptorSetLayoutBinding> image_bindings = {
        { 0, determine_descriptor_type("storage image"), 1, set_shader_stage_bits({ .compute = true }), 0 }
    };

    VkResult result = vkCreateDescriptorSetLayout(
        *get_hardware(),
        descriptor_layout_creation_info(buffer_bindings),
        nullptr,
        get_descriptor_buffer_layout()
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of buffer descriptor set layout failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
    set_descriptor_buffer_pool_size( { .type = determine_descriptor_type("storage buffer"), .descriptorCount = (uint32_t) buffer_bindings.size() } );

    result = vkCreateDescriptorSetLayout(
        *get_hardware(),
        descriptor_layout_creation_info(image_bindings),
        nullptr,
        get_descriptor_image_layout()
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of image descriptor set layout failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
    set_descriptor_image_pool_size( { .type = determine_descriptor_type("storage image"), .descriptorCount = *get_swapchain_image_count() } );
}

VkDescriptorType determine_descriptor_type (string name)
{
    VkDescriptorType type;
    if (name == "sampler") type = VK_DESCRIPTOR_TYPE_SAMPLER;
    if (name == "combined image sampler") type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    if (name == "sampled image") type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    if (name == "storage image") type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    if (name == "uniform texel buffer") type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
    if (name == "storage texel buffer") type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
    if (name == "uniform buffer") type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    if (name == "storage buffer") type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    if (name == "uniform buffer dynamic") type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    if (name == "storage buffer dynamic") type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
    if (name == "input attachment") type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    return type;
}

void initialize_descriptor_pools ()
{
    VkResult result = vkCreateDescriptorPool(
        *get_hardware(),
        descriptor_pool_creation_info( get_descriptor_buffer_pool_size() ),
        nullptr,
        get_descriptor_buffer_pool()
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of buffer descriptor pool failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );

    result = vkCreateDescriptorPool(
        *get_hardware(),
        descriptor_pool_creation_info( get_descriptor_image_pool_size() ),
        nullptr,
        get_descriptor_image_pool()
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of image descriptor pool failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
}

void initialize_descriptor_set ()
{
    VkResult result = vkAllocateDescriptorSets(
        *get_hardware(),
        descriptor_set_allocation_info( *get_descriptor_buffer_pool(), get_descriptor_buffer_layout() ),
        get_descriptor_buffer_set()
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Allocation of buffer descriptor set failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );

    vector<VkDescriptorSet> image_descriptor_sets = *get_descriptor_image_sets();
    image_descriptor_sets.resize( *get_swapchain_image_count() );

    for (int i = 0; i < *get_swapchain_image_count(); i++)
    {
        result = vkAllocateDescriptorSets(
            *get_hardware(),
            descriptor_set_allocation_info( *get_descriptor_image_pool(), get_descriptor_image_layout() ),
            &image_descriptor_sets[i]
        );
        throw_error(result != VK_SUCCESS,
            "\n"
            "ERROR: Allocation of image descriptor set " + to_string(i) + " failed.\n"
            "       The returned result is '" + get_result_string(result) + "'."
        );
    }

    set_descriptor_image_sets(image_descriptor_sets);

    vector<VkDescriptorImageInfo> swapchain_image_infos;
    vector<VkSampler> swapchain_image_samplers = *get_swapchain_image_samplers();
    vector<VkImageView> swapchain_image_views = *get_swapchain_image_views();
    for (int i = 0; i < *get_swapchain_image_count(); i++)
    {
        swapchain_image_infos.push_back(
            { swapchain_image_samplers[i], swapchain_image_views[i], VK_IMAGE_LAYOUT_GENERAL }
        );
    }

    vector<VkDescriptorBufferInfo> buffer_infos = {
        {
            *get_instance_buffer(),
            0,
            VK_WHOLE_SIZE
        },
        {
            *get_primitive_buffer(),
            0,
            VK_WHOLE_SIZE
        },
        {
            *get_blbihs_buffer(),
            0,
            VK_WHOLE_SIZE
        },
        {
            *get_tlbih_buffer(),
            0,
            VK_WHOLE_SIZE
        }
    };

    uint32_t destination_array_element = 0;
    uint32_t destination_descriptor_count = 1;
    vector<VkWriteDescriptorSet> write_descriptor_sets;
    for (int i = 0; i < get_descriptor_buffer_pool_size()->descriptorCount; i++)
    {
        write_descriptor_sets.push_back(
            {
                VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                nullptr,
                *get_descriptor_buffer_set(), 
                (uint32_t) i,
                destination_array_element, 
                destination_descriptor_count, 
                determine_descriptor_type("storage buffer"),
                nullptr,
                &buffer_infos[i]
            }
        );
    }
    for (int i = 0; i < *get_swapchain_image_count(); i++)
    {
        write_descriptor_sets.push_back(
            {
                VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                nullptr,
                image_descriptor_sets[i], 
                0, 
                destination_array_element, 
                destination_descriptor_count, 
                determine_descriptor_type("storage image"),
                &swapchain_image_infos[i],
                nullptr
            }
        );
    }

    vkUpdateDescriptorSets( *get_hardware(), write_descriptor_sets.size(), write_descriptor_sets.data(), 0, nullptr );
}

void cleanup_descriptor ()
{
    vkDestroyDescriptorPool( *get_hardware(), *get_descriptor_buffer_pool(), nullptr );
    vkDestroyDescriptorPool( *get_hardware(), *get_descriptor_image_pool(), nullptr );
    vkDestroyDescriptorSetLayout( *get_hardware(), *get_descriptor_buffer_layout(), nullptr);
    vkDestroyDescriptorSetLayout( *get_hardware(), *get_descriptor_image_layout(), nullptr);
}
