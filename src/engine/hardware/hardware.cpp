// INCLUDE
#include "./hardware.hpp"
#include "../memory/memory.hpp"
#include "../swapchain/swapchain.hpp"

// CODE
void initialize_hardware ()
{
    initialize_hardware_device();

    uint32_t graphics_queue_family = (*get_hardware_attributes())[ get_hardware_attribute_index() ].graphics_queue;
    uint32_t presentation_queue_family = (*get_hardware_attributes())[ get_hardware_attribute_index() ].presentation_queue;
    uint32_t compute_queue_family = (*get_hardware_attributes())[ get_hardware_attribute_index() ].compute_queue;

    vector<VkDeviceQueueCreateInfo> queue_creation_infos;
    set<uint32_t> unique_queue_families = {graphics_queue_family, presentation_queue_family, compute_queue_family};

    for (uint32_t queue_family : unique_queue_families)
    {
        queue_creation_infos.push_back( *queue_creation_info( queue_family ) );
    }

    VkResult result = vkCreateDevice( *get_hardware_device(), device_creation_info(queue_creation_infos), nullptr, get_hardware() );

    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Communication to graphics hardware could not be established.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );

    vkGetDeviceQueue( *get_hardware(), graphics_queue_family, 0, get_graphics_queue() );
    vkGetDeviceQueue( *get_hardware(), presentation_queue_family, 0, get_presentation_queue() );
    vkGetDeviceQueue( *get_hardware(), compute_queue_family, 0, get_compute_queue() );
}

void initialize_hardware_device ()
{
    uint32_t hardware_count = 0;
    vkEnumeratePhysicalDevices( *get_vulkan_instance(), &hardware_count, nullptr );

    throw_error(hardware_count == 0,
        "\n"
        "ERROR: No graphics hardware with vulkan support found.\n"
    );

    vector<VkPhysicalDevice> hardware_devices(hardware_count);
    vkEnumeratePhysicalDevices( *get_vulkan_instance(), &hardware_count, hardware_devices.data() );

    #if DEBUG
        cout << "\nAvailable graphics hardware:" << endl;
    #endif

    VkPhysicalDevice best_hardware;
    int32_t best_hardware_index;
    float highest_score = 0;
    float new_score;
    for (int i = 0; i < hardware_devices.size(); i++)
    {
        new_score = determine_hardware_suitability(hardware_devices[i]);
        if (new_score > highest_score)
        {
            highest_score = new_score;
            best_hardware = hardware_devices[i];
            best_hardware_index = i;
        }
    }
    set_hardware_device(best_hardware);
    set_hardware_attribute_index(best_hardware_index);

    #if DEBUG
        cout << "\nSelected graphics hardware:" << endl;
        VkPhysicalDeviceProperties best_hardware_properties;
        vkGetPhysicalDeviceProperties(best_hardware, &best_hardware_properties);
        cout << "\t'" << best_hardware_properties.deviceName << "'" << endl;
    #endif

    throw_error(get_hardware_device() == VK_NULL_HANDLE, 
        "\n"
        "ERROR: No suitable graphics hardware found."
    );
}

float determine_hardware_suitability (VkPhysicalDevice hardware_candidate)
{
    VkPhysicalDeviceProperties candidate_properties;
    vkGetPhysicalDeviceProperties(hardware_candidate, &candidate_properties);
    VkPhysicalDeviceLimits hardware_limits = candidate_properties.limits;

    VkPhysicalDeviceFeatures candidate_features;
    vkGetPhysicalDeviceFeatures(hardware_candidate, &candidate_features);

    static struct hardware_candidate_attributes attributes = {
        .maximum_shared_memory_size = hardware_limits.maxComputeSharedMemorySize,
        .maximum_work_group_count = hardware_limits.maxComputeWorkGroupCount,
        .maximum_work_group_size = hardware_limits.maxComputeWorkGroupSize,
        .maximum_uniform_buffer_range = hardware_limits.maxUniformBufferRange,
        .maximum_storage_buffer_range = hardware_limits.maxStorageBufferRange,
        .maximum_push_constant_size = hardware_limits.maxPushConstantsSize,
        .minimum_uniform_buffer_offset_alignment = hardware_limits.minStorageBufferOffsetAlignment,
        .minimum_storage_buffer_offset_alignment = hardware_limits.minUniformBufferOffsetAlignment
    };

    #if DEBUG
        cout << "\t" << candidate_properties.deviceName << endl;
        cout << "\t\t1D image dimension limit: " + to_string(hardware_limits.maxImageDimension1D) + " pixels" << endl;
        cout << "\t\t2D image dimension limit: " + to_string(hardware_limits.maxImageDimension2D) + " pixels" << endl;
        cout << "\t\t3D image dimension limit: " + to_string(hardware_limits.maxImageDimension3D) + " pixels" << endl;
        cout << "\t\tMaximum uniform buffer range: " + to_string(attributes.maximum_uniform_buffer_range) + " bytes" << endl;
        cout << "\t\tMinimum uniform buffer offset alignment: " + to_string(attributes.minimum_uniform_buffer_offset_alignment) + " bytes" << endl;
        cout << "\t\tMaximum storage buffer range: " + to_string(attributes.maximum_storage_buffer_range) + " bytes" << endl;
        cout << "\t\tMinimum storage buffer offset alignment: " + to_string(attributes.minimum_storage_buffer_offset_alignment) + " bytes" << endl;
        cout << "\t\tMaximum push constant size: " + to_string(attributes.maximum_push_constant_size) + " bytes" << endl;
        cout << "\t\tMaximum shared compute memory: " + to_string(attributes.maximum_shared_memory_size) + " bytes" << endl;
        cout << "\t\tMaximum compute work group count:\n\t\t\tx: " + to_string(attributes.maximum_work_group_count[0]) + "\n\t\t\ty: " + to_string(attributes.maximum_work_group_count[1]) + "\n\t\t\tz: " + to_string(attributes.maximum_work_group_count[2]) << endl;
        cout << "\t\tMaximum compute work group invocations: " + to_string(hardware_limits.maxComputeWorkGroupInvocations) << endl;
        cout << "\t\tMaximum compute work group size:\n\t\t\tx: " + to_string(attributes.maximum_work_group_size[0]) + "\n\t\t\ty: " + to_string(attributes.maximum_work_group_size[1]) + "\n\t\t\tz: " + to_string(attributes.maximum_work_group_size[2]) << endl;
    #endif

    queue_family_indices indices = find_queue_families( hardware_candidate, true );
    swapchain_support_details swap_chain_support = determine_swapchain_support_details( hardware_candidate, true );

    unsigned int queue_families_found = indices.is_complete() ? 1 : 0;
    unsigned int extensions_supported = are_hardware_extensions_supported(hardware_candidate) ? 1 : 0;
    unsigned int surface_supported = !swap_chain_support.surface_formats.empty() ? 1 : 0;
    unsigned int presentation_mode_supported = !swap_chain_support.presentation_modes.empty() ? 1 : 0;
    float storage_buffer_space = float(attributes.maximum_storage_buffer_range) / float(10000000000);

    attributes.score = queue_families_found + extensions_supported + surface_supported + presentation_mode_supported + storage_buffer_space;

    attributes.graphics_queue = indices.graphics_family.value();
    attributes.presentation_queue = indices.presentation_family.value();
    attributes.compute_queue = indices.compute_family.value();

    vector<hardware_candidate_attributes> hardware_attributes = *get_hardware_attributes();
    hardware_attributes.resize( hardware_attributes.size() + 1 );
    hardware_attributes.push_back( attributes );
    set_hardware_attributes( hardware_attributes );

    #if DEBUG
        cout << "\t\tSelected queue families:" << endl;
        set<uint32_t> queue_family_indices = {attributes.graphics_queue, attributes.presentation_queue, attributes.compute_queue};
        for (const uint32_t queue_family_index : queue_family_indices)
        {
            cout << "\t\t\t" << queue_family_index << endl;
        }
        cout << "\t\tScore: " << to_string(attributes.score) << endl;
    #endif

    return attributes.score;
}

bool are_hardware_extensions_supported (VkPhysicalDevice hardware_candidate)
{
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(hardware_candidate, nullptr, &extension_count, nullptr);
    vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(hardware_candidate, nullptr, &extension_count, available_extensions.data());

    vector<const char*> hardware_extensions_copy = *get_hardware_extensions();
    set<string> required_extensions( hardware_extensions_copy.begin(), hardware_extensions_copy.end() );

    for (const auto& extension : available_extensions)
    {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

void cleanup_hardware ()
{
    vkDestroyDevice( *get_hardware(), nullptr);
}
