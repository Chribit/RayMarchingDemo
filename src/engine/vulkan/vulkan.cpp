// INCLUDE
#include "./vulkan.hpp"

// CODE
void initialize_vulkan ()
{
    initialize_validation_layers();
    initialize_instance_extensions();

    VkResult creation_result = vkCreateInstance( instance_creation_info(), nullptr, get_vulkan_instance() );

    throw_error(creation_result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of a vulkan instance failed.\n"
        "       The returned result is " + get_result_string(creation_result) + "."
    );
}

void initialize_instance_extensions ()
{
    vector<const char*> extensions = *get_vulkan_extensions();
    uint32_t extension_count = 0;   
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extension_count);

    for (int e = 0; e < extension_count; e++)
    {
        extensions.push_back(glfw_extensions[e]);
    }
    
    #if FOR_MAC
        extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    #endif

    set_vulkan_extensions(extensions);
}

void cleanup_vulkan ()
{
    vkDestroyInstance( *get_vulkan_instance(), nullptr );
}
