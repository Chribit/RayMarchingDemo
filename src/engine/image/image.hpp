// INCLUDE
#include "../window/window.hpp"

// DATA
#ifndef DEFINED_IUB
#define DEFINED_IUB
struct image_usage_bits {
    bool transfer_source = false;
    bool transfer_destination = false;
    bool sampled = false;
    bool storage = false;
    bool colour_attachment = false;
    bool depth_stencil_attachment = false;
    bool transient_attachment = false;
    bool input_attachment = false;
};
#endif
#ifndef DEFINED_IFFB
#define DEFINED_IFFB
struct image_format_feature_bits {
    bool sampled_image = false;
    bool storage_image = false;
    bool atomic_storage_image = false;
    bool uniform_texel_buffer = false;
    bool storage_texel_buffer = false;
    bool atomic_storage_texel_buffer = false;
    bool vertex_buffer = false;
    bool colour_attachment = false;
    bool colour_attachment_blend = false;
    bool depth_stencil_attachment = false;
    bool blit_source = false;
    bool blit_destination = false;
    bool sampled_linear_image_filter = false;
};
#endif

// CODE
void initialize_image ();
void initialize_swapchain_images ();
void initialize_swapchain_image_samplers ();
void initialize_swapchain_image_views ();
void command_image_layout_transition (VkCommandBuffer command_buffer, VkImageLayout old_layout, VkImageLayout new_layout);
void determine_image_extent ();
uint32_t set_image_usage_bits (image_usage_bits given_bits);
image_format_feature_bits get_image_format_feature_bits (VkFormatFeatureFlags given_flags);
void cleanup_image ();

// ACCESS
vector<VkImage>* get_swapchain_images ();
void set_swapchain_images (vector<VkImage> swapchain_images);
vector<VkSampler>* get_swapchain_image_samplers ();
void set_swapchain_image_samplers (vector<VkSampler> swapchain_image_samplers);
vector<VkImageView>* get_swapchain_image_views ();
void set_swapchain_image_views (vector<VkImageView> swapchain_image_views);
VkFormat* get_swapchain_image_format ();
void set_swapchain_image_format (VkFormat format);
VkColorSpaceKHR* get_swapchain_image_colour_space ();
void set_swapchain_image_colour_space (VkColorSpaceKHR colour_space);
VkPresentModeKHR* get_swapchain_image_presentation_mode ();
void set_swapchain_image_presentation_mode (VkPresentModeKHR presentation_mode);
VkExtent2D* get_swapchain_image_extent ();
void set_swapchain_image_extent (VkExtent2D extent);
