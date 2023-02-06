// INCLUDE
#include "./window.hpp"
#include "../image/image.hpp"

// DATA
static struct {
    GLFWwindow* window;
    uint32_t initial_width;
    uint32_t initial_height;
    VkSurfaceKHR surface;
    bool is_resized = false;
} window;

// CODE
GLFWwindow* get_window ()
{
    return window.window;
}

void set_window (GLFWwindow* glfw_window)
{
    window.window = glfw_window;
}

VkSurfaceKHR* get_window_surface ()
{
    return &window.surface;
}

void set_window_surface (VkSurfaceKHR surface)
{
    window.surface = surface;
}

glm::uvec2 get_initial_window_size ()
{
    return {window.initial_width, window.initial_height};
}

void get_window_size (int* width, int* height)
{
    glfwGetWindowSize( get_window(), width, height );
}

void get_window_pixel_size (int* width, int* height)
{
    glfwGetFramebufferSize( get_window(), width, height );
}

void set_initial_window_size (glm::uvec2 size)
{
    window.initial_width = size.x;
    window.initial_height = size.y;
}

void set_window_size (uint32_t width, uint32_t height)
{
    glfwSetWindowSize(window.window, width, height);
}

void set_window_title (string title)
{
    glfwSetWindowTitle(window.window, title.c_str());
}

bool get_window_resized ()
{
    return window.is_resized;
}

void set_window_resized (bool resized)
{
    window.is_resized = resized;
}
