// INCLUDE
#include "./image.hpp"
#include "../hardware/hardware.hpp"
#include "../swapchain/swapchain.hpp"

// CODE
void initialize_image ()
{
    initialize_swapchain_images();
    initialize_swapchain_image_samplers();
    initialize_swapchain_image_views();
}

void initialize_swapchain_images ()
{
    vector<VkImage> swapchain_images = *get_swapchain_images();
    swapchain_images.resize( *get_swapchain_image_count() );

    VkResult result = vkGetSwapchainImagesKHR(
        *get_hardware(),
        *get_swapchain(),
        get_swapchain_image_count(),
        swapchain_images.data()
    );

    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Acquisition of swapchain image failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );

    set_swapchain_images( swapchain_images );
}

void initialize_swapchain_image_samplers ()
{
    // OPTIMISE: maybe no need to recreate samplers

    vector<VkSampler> swapchain_image_samplers = *get_swapchain_image_samplers();
    swapchain_image_samplers.resize( *get_swapchain_image_count() );

    for (int i = 0; i < *get_swapchain_image_count(); i++)
    {
        VkResult result = vkCreateSampler(
            *get_hardware(),
            image_sampler_creation_info(false),
            nullptr,
            &swapchain_image_samplers[i]
        );

        throw_error(result != VK_SUCCESS,
            "\n"
            "ERROR: Creation of swapchain image sampler " + to_string(i) + " failed.\n"
            "       The returned result is '" + get_result_string(result) + "'."
        );
    }

    set_swapchain_image_samplers( swapchain_image_samplers );
}

void initialize_swapchain_image_views ()
{
    vector<VkImage> swapchain_images = *get_swapchain_images();
    vector<VkImageView> swapchain_image_views = *get_swapchain_image_views();
    swapchain_image_views.resize( *get_swapchain_image_count() );

    for (int i = 0; i < *get_swapchain_image_count(); i++)
    {
        VkResult result = vkCreateImageView(
            *get_hardware(),
            image_view_creation_info( swapchain_images[i], *get_swapchain_image_format()),
            nullptr,
            &swapchain_image_views[i]
        );

        throw_error(result != VK_SUCCESS,
            "\n"
            "ERROR: Creation of swapchain image view " + to_string(i) + " failed.\n"
            "       The returned result is '" + get_result_string(result) + "'."
        );
    }

    set_swapchain_image_views( swapchain_image_views );
}

void command_image_layout_transition (VkCommandBuffer command_buffer, VkImageLayout old_layout, VkImageLayout new_layout)
{
    VkImage image = (*get_swapchain_images())[ *get_swapchain_image_index() ];

    VkImageSubresourceRange subresource_range = {};
    subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresource_range.baseMipLevel = 0;
    subresource_range.levelCount = 1;
    subresource_range.layerCount = 1;

    VkImageMemoryBarrier image_barrier = {};
    image_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    image_barrier.oldLayout = old_layout;
    image_barrier.newLayout = new_layout;
    image_barrier.image = image;
    image_barrier.subresourceRange = subresource_range;

    VkPipelineStageFlags source_flags = 0;
    switch (old_layout)
    {
        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            source_flags = VK_ACCESS_HOST_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            source_flags = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            source_flags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            source_flags = VK_ACCESS_TRANSFER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            source_flags = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            source_flags = VK_ACCESS_SHADER_READ_BIT;
            break;

        default:
            break;
    }

    VkPipelineStageFlags destination_flags = 0;
    switch (new_layout)
    {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            destination_flags = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            destination_flags = VK_ACCESS_TRANSFER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            destination_flags = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            destination_flags = image_barrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            if (image_barrier.srcAccessMask == 0) source_flags = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
            destination_flags = VK_ACCESS_SHADER_READ_BIT;
            break;

        default:
            break;
    }

    image_barrier.srcAccessMask = source_flags;
    image_barrier.dstAccessMask = destination_flags;
    VkPipelineStageFlagBits stage_flag = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

    vkCmdPipelineBarrier( command_buffer, stage_flag, stage_flag, 0, 0, nullptr, 0, nullptr, 1, &image_barrier);
}

void determine_image_extent ()
{
    VkExtent2D extent;

    int width, height;
    get_window_size( &width, &height );

    extent = {
        .width = static_cast<uint32_t>(width), 
        .height = static_cast<uint32_t>(height)
    };

    extent.width = clamp(extent.width, (uint32_t) 10, (uint32_t) 5000);
    extent.height = clamp(extent.height, (uint32_t) 10, (uint32_t) 5000);

    set_swapchain_image_extent(extent);
}

uint32_t set_image_usage_bits (image_usage_bits given_bits)
{
    uint32_t bits = 0;
    if (given_bits.transfer_source) bits += 1;
    if (given_bits.transfer_destination) bits += 2;
    if (given_bits.sampled) bits += 4;
    if (given_bits.storage) bits += 8;
    if (given_bits.colour_attachment) bits += 16;
    if (given_bits.depth_stencil_attachment) bits += 32;
    if (given_bits.transient_attachment) bits += 64;
    if (given_bits.input_attachment) bits += 128;
    return bits;
}

image_format_feature_bits get_image_format_feature_bits (VkFormatFeatureFlags given_flags)
{
    image_format_feature_bits bits;
    if (given_flags & 0x1000) bits.sampled_linear_image_filter = true;
    if (given_flags & 0x0800) bits.blit_destination = true;
    if (given_flags & 0x0400) bits.blit_source = true;
    if (given_flags & 0x0200) bits.depth_stencil_attachment = true;
    if (given_flags & 0x0100) bits.colour_attachment_blend = true;
    if (given_flags & 0x0080) bits.colour_attachment = true;
    if (given_flags & 0x0040) bits.vertex_buffer = true;
    if (given_flags & 0x0020) bits.atomic_storage_texel_buffer = true;
    if (given_flags & 0x0010) bits.storage_texel_buffer = true;
    if (given_flags & 0x0008) bits.uniform_texel_buffer = true;
    if (given_flags & 0x0004) bits.atomic_storage_image = true;
    if (given_flags & 0x0002) bits.storage_image = true;
    if (given_flags & 0x0001) bits.sampled_image = true;
    return bits;
}

void cleanup_image ()
{
    vector<VkSampler> image_samplers = *get_swapchain_image_samplers();
    vector<VkImageView> image_views = *get_swapchain_image_views();
    for (int i = 0; i < *get_swapchain_image_count(); i++)
    {
        vkDestroySampler( *get_hardware(), image_samplers[i], nullptr );
        vkDestroyImageView( *get_hardware(), image_views[i], nullptr );
    }
}
