// INCLUDE
#include "../window/window.hpp"
#include "../structure/structure.hpp"
#include "../compact/compact.hpp"
#include "../image/image.hpp"
#include "../synchronisation/synchronisation.hpp"

// DATA
#ifndef DEFINED_SCSD
#define DEFINED_SCSD
struct swapchain_support_details {
    VkSurfaceCapabilitiesKHR surface_capabilities;
    vector<VkSurfaceFormatKHR> surface_formats;
    vector<VkPresentModeKHR> presentation_modes;
};
#endif

// CODE
void initialize_swapchain (bool recreation);
swapchain_support_details determine_swapchain_support_details (VkPhysicalDevice hardware, bool candidate);
void recreate_swapchain ();
void choose_swapchain_surface_format (const vector<VkSurfaceFormatKHR>& available_formats);
void choose_swapchain_presentation_mode (const vector<VkPresentModeKHR>& available_presentation_modes);
void acquire_next_swapchain_image (uint32_t swapchain_frame);
void cleanup_swapchain ();

// ACCESS
VkSwapchainKHR* get_swapchain ();
void set_swapchain (VkSwapchainKHR swap_chain);
swapchain_support_details* get_swapchain_support_details ();
void set_swapchain_support_details (swapchain_support_details details);
uint32_t* get_swapchain_image_count ();
void set_swapchain_image_count (uint32_t image_count);
uint32_t* get_swapchain_image_index ();
void set_swapchain_image_index (uint32_t index);
