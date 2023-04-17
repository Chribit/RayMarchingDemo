// INCLUDE
#include "./swapchain.hpp"
#include "../hardware/hardware.hpp"
#include "../shading/shading.hpp"

// CODE
void initialize_swapchain (bool recreation)
{
    if (!recreation) 
    {
        determine_swapchain_support_details( *get_hardware_device(), false );
    }

    swapchain_support_details swapchain_support = *get_swapchain_support_details();

    if (!recreation)
    {
        choose_swapchain_surface_format( swapchain_support.surface_formats );
        choose_swapchain_presentation_mode( swapchain_support.presentation_modes );
        set_swapchain_image_count( swapchain_support.surface_capabilities.minImageCount );
    }

    determine_image_extent();

    VkResult result = vkCreateSwapchainKHR(
        *get_hardware(),
        swapchain_creation_info(),
        nullptr,
        get_swapchain()
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Swap chain creation failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );

    initialize_image();
}

swapchain_support_details determine_swapchain_support_details (VkPhysicalDevice hardware, bool candidate)
{
    swapchain_support_details details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR( hardware, *get_window_surface(), &details.surface_capabilities );

    uint32_t surface_format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR( hardware, *get_window_surface(), &surface_format_count, nullptr );
    if (surface_format_count != 0)
    {
        details.surface_formats.resize(surface_format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR( hardware, *get_window_surface(), &surface_format_count, details.surface_formats.data());
    }

    uint32_t presentation_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR( hardware, *get_window_surface(), &presentation_mode_count, nullptr);
    if (presentation_mode_count != 0)
    {
        details.presentation_modes.resize(presentation_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR( hardware, *get_window_surface(), &presentation_mode_count, details.presentation_modes.data());
    }

    if (!candidate)
    {
        set_swapchain_support_details(details);
        return details;
    }
    else
    {
        return details;
    }
}

void choose_swapchain_surface_format (const vector<VkSurfaceFormatKHR>& available_formats)
{
    bool format_found = false;

    #if DEBUG
        cout << "\nAvailable surface formats (format - colour space):" << endl;
    #endif

    VkSurfaceFormatKHR surface_format;
    VkFormatProperties surface_format_properties;

    for (int i = 0; i < available_formats.size(); i++)
    {
        surface_format = available_formats[i];

        vkGetPhysicalDeviceFormatProperties( *get_hardware_device(), surface_format.format, &surface_format_properties);
        image_format_feature_bits features[3] = {
            get_image_format_feature_bits( surface_format_properties.linearTilingFeatures ),
            get_image_format_feature_bits( surface_format_properties.optimalTilingFeatures ),
            get_image_format_feature_bits( surface_format_properties.bufferFeatures )
        };

        #if DEBUG
            cout << "\t" << get_format_string(surface_format.format) << " - " << get_colour_space_string(surface_format.colorSpace) << endl;
            string feature_names[3] = {"Linear tiling", "Optimal tiling", "Buffer"};
            for (int j = 0; j < 3; j++)
            {
                cout << "\t\t\t" + feature_names[j] + ":" << endl;
                cout << "\t\t\t\tSampled image: " << (features[j].sampled_image ? "true" : "false") << endl;
                cout << "\t\t\t\tStorage image: " << (features[j].storage_image ? "true" : "false") << endl;
                cout << "\t\t\t\tAtomic storage image: " << (features[j].atomic_storage_image ? "true" : "false") << endl;
                cout << "\t\t\t\tUniform texel buffer: " << (features[j].uniform_texel_buffer ? "true" : "false") << endl;
                cout << "\t\t\t\tStorage texel buffer: " << (features[j].storage_texel_buffer ? "true" : "false") << endl;
                cout << "\t\t\t\tAtomic storage texel buffer: " << (features[j].atomic_storage_texel_buffer ? "true" : "false") << endl;
                cout << "\t\t\t\tVertex buffer: " << (features[j].vertex_buffer ? "true" : "false") << endl;
                cout << "\t\t\t\tColour attachment: " << (features[j].colour_attachment ? "true" : "false") << endl;
                cout << "\t\t\t\tColour attachment blend: " << (features[j].colour_attachment_blend ? "true" : "false") << endl;
                cout << "\t\t\t\tDepth stencil attachment: " << (features[j].depth_stencil_attachment ? "true" : "false") << endl;
                cout << "\t\t\t\tBlit source: " << (features[j].blit_source ? "true" : "false") << endl;
                cout << "\t\t\t\tBlit destination: " << (features[j].blit_destination ? "true" : "false") << endl;
                cout << "\t\t\t\tSampled linear image filter: " << (features[j].sampled_linear_image_filter ? "true" : "false") << endl;
            }
        #endif

        if (features[1].storage_image && get_format_string(surface_format.format).find("B8G8R8A8") != -1 )
        {
            set_swapchain_image_format(surface_format.format);
            set_swapchain_image_colour_space(surface_format.colorSpace);
            format_found = true;
        }
    }

    #if DEBUG
        cout << "\nSelected surface format:" << endl;
        cout << "\t" << get_format_string( *get_swapchain_image_format() ) << " - " << get_colour_space_string( *get_swapchain_image_colour_space() ) << endl;
    #endif

    throw_error(!format_found,
        "\n"
        "ERROR: No suitable surface format found.\n"
        "       Check available surface formats in development build."
    );
}

void choose_swapchain_presentation_mode (const vector<VkPresentModeKHR>& available_presentation_modes)
{   
    bool mailbox_found = false;
    bool fifo_found = false;

    #if DEBUG
        cout << "\nAvailable presentation modes:" << endl;
    #endif

    VkPresentModeKHR presentation_mode;

    for (int i = 0; i < available_presentation_modes.size(); i++)
    {
        presentation_mode = available_presentation_modes[i];

        #if DEBUG
            cout << "\t" << get_presentation_mode_string( presentation_mode ) << endl;
        #endif

        if (presentation_mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            set_swapchain_image_presentation_mode(presentation_mode);
            mailbox_found = true;
        }
        if (presentation_mode == VK_PRESENT_MODE_FIFO_KHR && !mailbox_found)
        {
            set_swapchain_image_presentation_mode(presentation_mode);
            fifo_found = true;
        }
    }

    #if DEBUG
        cout << "\nSelected presentation mode:" << endl;
        cout << "\t" << get_presentation_mode_string( *get_swapchain_image_presentation_mode() ) << endl;
    #endif

    throw_error(!fifo_found && !mailbox_found,
        "\n"
        "ERROR: Neither MAILBOX nor FIFO presentation mode found.\n"
        "       Check available presentation modes in development build."
    );
}

void recreate_swapchain ()
{
    int width;
    int height;
    get_window_size( &width, &height );

    while ( is_window_minimized( &width, &height ) )
    {
        get_window_size( &width, &height );
        glfwWaitEvents();
    }

    vkDeviceWaitIdle( *get_hardware() );

    cleanup_descriptor();
    cleanup_swapchain();

    initialize_swapchain(true);
    initialize_descriptor();

    set_window_resized(false);
}

void acquire_next_swapchain_image (uint32_t swapchain_frame)
{
    VkResult result = vkAcquireNextImageKHR(
        *get_hardware(),
        *get_swapchain(),
        1 SECOND,
        *get_gpu_image_available( (int) swapchain_frame ),
        VK_NULL_HANDLE,
        get_swapchain_image_index()
    );
    
    if (result == VK_ERROR_OUT_OF_DATE_KHR) recreate_swapchain();
    else throw_error(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR,
            "\n"
            "ERROR: Acquisition of current frame's swapchain image failed.\n"
            "       The returned result is '" + get_result_string(result) + "'."
         );
}

void cleanup_swapchain ()
{
    vkDestroySwapchainKHR( *get_hardware(), *get_swapchain(), nullptr);
    cleanup_image();
}
