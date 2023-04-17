// INCLUDE
#include "./window.hpp"
#include "../engine.hpp"
#include "../event/event.hpp"

// CODE
void initialize_window ()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glm::uvec2 window_size = get_initial_window_size();

    set_window( glfwCreateWindow( window_size.x, window_size.y, get_application_name().c_str(), nullptr, nullptr ) );
    glfwSetWindowUserPointer( get_window(), NULL );

    glfwSetWindowSizeCallback( get_window(), on_window_resize );

    initialize_vulkan();

    VkResult result = glfwCreateWindowSurface( *get_vulkan_instance(), get_window(), nullptr, get_window_surface() );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of window surface failed.\n"
        "       The function create_window_surface returned the error '" + get_result_string(result) + "'."
    );
}

void on_window_resize (GLFWwindow* window, int width, int height)
{
    set_push_constant_window_size( glm::ivec2(width, height) );

    run_window_resize_callbacks();
    render();
    set_window_resized(true);

    #if DEBUG
        cout << "\tResized window to dimensions " << width << " x " << height << "." << endl;
    #endif
}

bool is_window_minimized (int* width, int* height)
{
    return *width == 0 || *height == 0;
}

bool is_window_outdated (VkResult presentation_result)
{
    return presentation_result == VK_ERROR_OUT_OF_DATE_KHR || presentation_result == VK_SUBOPTIMAL_KHR || get_window_resized();
}

void present_image (int swapchain_frame, uint32_t* swapchain_image_index)
{
    VkResult result = vkQueuePresentKHR(
        *get_presentation_queue(),
        present_info( (uint32_t) swapchain_frame, swapchain_image_index )
    );
    
    if ( is_window_outdated(result) ) 
    {
        set_swapchain_frame(1);     // set swapchain frame to 1 so next render is for 0
        recreate_swapchain();
    }
    else throw_error(result != VK_SUCCESS,
            "\n"
            "ERROR: Presentation of swapchain image failed.\n"
            "       The returned result is '" + get_result_string(result) + "'."
         );
}

void cleanup_window ()
{
    vkDestroySurfaceKHR( *get_vulkan_instance(), *get_window_surface(), nullptr );
    glfwDestroyWindow( get_window() );
    glfwTerminate();
}
