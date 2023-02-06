// INCLUDE
#include "../compact/compact.hpp"
#include "../vulkan/vulkan.hpp"

// CODE
void initialize_window ();
void on_window_resize (GLFWwindow* window, int width, int height);
bool is_window_minimized (int* width, int* height);
bool is_window_outdated (VkResult presentation_result);
void present_image (int swapchain_frame, uint32_t* swapchain_image_index);
void cleanup_window ();

// ACCESS
GLFWwindow* get_window ();
void set_window (GLFWwindow* window);
VkSurfaceKHR* get_window_surface ();
void set_window_surface (VkSurfaceKHR surface);
glm::uvec2 get_initial_window_size ();
void get_window_size (int* width, int* height);
void get_window_pixel_size (int* width, int* height);
void set_initial_window_size (glm::uvec2 size);
void set_window_size (uint32_t width, uint32_t height);
void set_window_title (string title);
bool get_window_resized ();
void set_window_resized (bool resized);
