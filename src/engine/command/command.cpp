// INCLUDE
#include "./command.hpp"
#include "../hardware/hardware.hpp"
#include "../shading/shading.hpp"
#include "../descriptor/descriptor.hpp"
#include "../synchronisation/synchronisation.hpp"
#include "../swapchain/swapchain.hpp"
#include "../object/object.hpp"

// CODE
void initialize_command ()
{
    create_command_pool();
    create_command_buffers();
}

queue_family_indices find_queue_families (VkPhysicalDevice hardware_candidate, bool candidate)
{
    static queue_family_indices family_indices;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(hardware_candidate, &queue_family_count, nullptr);
    vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(hardware_candidate, &queue_family_count, queue_families.data());

    #if DEBUG
        cout << "\t\tQueue families:" << endl;
    #endif

    for (int i = 0; i < queue_families.size(); i++)
    {
        VkQueueFamilyProperties queue_family_properties = queue_families[i];
        queue_flag_bits bits = get_queue_bits( queue_family_properties.queueFlags );

        #if DEBUG
            cout << "\t\t\t" + to_string(i) + ":" << endl;
            cout << "\t\t\t\tCount: " + to_string(queue_family_properties.queueCount) << endl;
            cout << "\t\t\t\tGraphics: " << (bits.graphics ? "true" : "false") << endl;
            cout << "\t\t\t\tCompute: " << (bits.compute ? "true" : "false") << endl;
            cout << "\t\t\t\tTransfer: " << (bits.transfer ? "true" : "false") << endl;
            cout << "\t\t\t\tSparse Binding: " << (bits.sparse_binding ? "true" : "false") << endl;
        #endif

        if (family_indices.is_complete()) continue;

        if ( bits.graphics ) family_indices.graphics_family = i;
        if ( bits.compute ) family_indices.compute_family = i;
        if ( bits.transfer ) family_indices.transfer_family = i;
        if ( determine_queue_supports_presentation(hardware_candidate, i) ) family_indices.presentation_family = i;
    }
    
    return family_indices;
}

queue_flag_bits get_queue_bits (VkQueueFlags given_flags)
{
    queue_flag_bits bits;
    if (given_flags & 0x0008) bits.sparse_binding = true;
    if (given_flags & 0x0004) bits.transfer = true;
    if (given_flags & 0x0002) bits.compute = true;
    if (given_flags & 0x0001) bits.graphics = true;
    return bits;
}

bool determine_queue_supports_presentation (VkPhysicalDevice hardware, int queue_index)
{
    VkBool32 supported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(hardware, queue_index, *get_window_surface(), &supported);
    return supported;
}

void create_command_pool ()
{
    VkResult result = vkCreateCommandPool( *get_hardware(), command_pool_creation_info(), nullptr, get_command_pool() );

    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of command pool failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
}

void create_command_buffers ()
{
    vector<VkCommandBuffer> command_buffers = *get_command_buffers();
    command_buffers.resize( *get_swapchain_image_count() );

    VkResult result = vkAllocateCommandBuffers(
        *get_hardware(),
        command_buffer_allocation_info( *get_swapchain_image_count() ),
        command_buffers.data()
    );

    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Allocation of command buffers for swapchain failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );

    set_command_buffers( command_buffers );
}

void record_command_buffer (int swapchain_frame)
{
    reset_command_buffer(swapchain_frame);

    VkCommandBuffer command_buffer = *get_command_buffer(swapchain_frame);

    VkResult result = vkBeginCommandBuffer( command_buffer, command_buffer_begin_info() );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Command buffer recording failed to begin.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );

    vkCmdBindPipeline(
        command_buffer,
        VK_PIPELINE_BIND_POINT_COMPUTE,
        *get_shading_pipeline()
    );

    determine_scene_aabb();

    vkCmdPushConstants(
        command_buffer,
        *get_shading_pipeline_layout(),
        set_shader_stage_bits({ .compute = true }),
        get_push_constant_range()->offset,
        get_push_constant_range()->size,
        get_push_constants()
    );

    transmit_object_data(command_buffer);

    vector<VkDescriptorSet> descriptor_sets = get_descriptor_sets(swapchain_frame);
    vkCmdBindDescriptorSets(
        command_buffer,
        VK_PIPELINE_BIND_POINT_COMPUTE,
        *get_shading_pipeline_layout(),
        0,
        descriptor_sets.size(),
        descriptor_sets.data(),
        0,
        {}
    );

    command_image_layout_transition(
        command_buffer,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_GENERAL
    );

    int width;
    int height;
    get_window_size(&width, &height);

    #if DEBUG
        cout << "\tDispatching " << determine_workgroup_count(width) << " x " << determine_workgroup_count(height) << " work groups." << endl;
    #endif

    vkCmdDispatch(
        command_buffer,
        determine_workgroup_count(width),
        determine_workgroup_count(height),
        1
    );

    command_image_layout_transition(
        command_buffer,
        VK_IMAGE_LAYOUT_GENERAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    );

    result = vkEndCommandBuffer(command_buffer);
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Command buffer recording failed to end.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
}

void reset_command_buffer (int swapchain_frame)
{
    VkResult result = vkResetCommandBuffer( *get_command_buffer(swapchain_frame), 0 );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Command buffer reset failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
}

void submit_command_buffer (int swapchain_frame)
{
    VkResult result = vkQueueSubmit(
        *get_compute_queue(),
        1,
        submit_info( swapchain_frame ),
        *get_cpu_image_presented(swapchain_frame)
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Submission of command buffer failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
}

void cleanup_command ()
{
    vkDestroyCommandPool( *get_hardware(), *get_command_pool(), nullptr);
}
