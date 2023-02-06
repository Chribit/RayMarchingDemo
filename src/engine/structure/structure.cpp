// INCLUDE
#include "./structure.hpp"
#include "../engine.hpp"
#include "../hardware/hardware.hpp"
#include "../window/window.hpp"
#include "../image/image.hpp"
#include "../command/command.hpp"
#include "../vulkan/vulkan.hpp"

// CODE
APPLICATION* application_info ()
{
    static APPLICATION info {};
    info.sType = get_structure_type("application");
    info.pApplicationName = get_application_name().c_str();
    info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    info.pEngineName = get_engine_name().c_str();
    info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    info.apiVersion = VK_API_VERSION_1_0;
    return &info;
}

INSTANCE* instance_creation_info ()
{
    static vector<const char*> extensions = *get_vulkan_extensions();
    static vector<char*> validation_layers = *get_validation_layers();
    static INSTANCE info {};
    info.sType = get_structure_type("instance creation");
    info.pApplicationInfo = application_info();
    info.enabledExtensionCount = extensions.size();
    info.ppEnabledExtensionNames = extensions.data();
    info.enabledLayerCount = USE_VALIDATION_LAYERS ? get_validation_layer_count() : 0;
    info.ppEnabledLayerNames = USE_VALIDATION_LAYERS ? validation_layers.data() : nullptr;
    #if FOR_MAC
        info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif
    return &info;
}

QUEUE* queue_creation_info (uint32_t queue_family)
{
    static float priority = 1.0f;
    static QUEUE info {};
    info.sType = get_structure_type("queue creation");
    info.queueFamilyIndex = queue_family;
    info.queueCount = 1;
    info.pQueuePriorities = &priority;
    return &info;
}

DEVICE* device_creation_info (vector<VkDeviceQueueCreateInfo>& queue_creation_infos)
{
    static vector<const char*> hardware_extensions = *get_hardware_extensions();
    static vector<char*> validation_layers = *get_validation_layers();
    static DEVICE info {};
    info.sType = get_structure_type("device creation");
    info.pQueueCreateInfos = queue_creation_infos.data();
    info.queueCreateInfoCount = static_cast<uint32_t>( queue_creation_infos.size() );
    info.pEnabledFeatures = get_hardware_features();
    info.enabledExtensionCount = hardware_extensions.size();
    info.ppEnabledExtensionNames = hardware_extensions.data();
    info.enabledLayerCount = USE_VALIDATION_LAYERS ? get_validation_layer_count() : 0;
    info.ppEnabledLayerNames = USE_VALIDATION_LAYERS ? validation_layers.data() : nullptr;
    return &info;
}

BUFFER* buffer_creation_info (uint64_t buffer_size, uint32_t usage_flags, bool exclusive)
{
    static const uint32_t compute_queue_family_index = (*get_hardware_attributes())[ get_hardware_attribute_index() ].compute_queue;
    static BUFFER info {};
    info.sType = get_structure_type("buffer creation");
    info.size = buffer_size;
    info.usage = usage_flags;
    info.sharingMode = exclusive ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
    info.queueFamilyIndexCount = 1;
    info.pQueueFamilyIndices = &compute_queue_family_index;
    return &info;
}

IMAGE_SAMPLER* image_sampler_creation_info (bool normalized_coordinates)
{
    static IMAGE_SAMPLER info {};
    info.sType = get_structure_type("image sampler creation");
    info.flags = 0;
    info.magFilter = VK_FILTER_NEAREST;
    info.minFilter = VK_FILTER_NEAREST;
    info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    info.anisotropyEnable = VK_FALSE;
    info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    info.unnormalizedCoordinates = !normalized_coordinates;
    info.compareEnable = VK_FALSE;
    info.compareOp = VK_COMPARE_OP_NEVER;
    info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    info.mipLodBias = 0.0f;
    info.minLod = 0.0f;
    info.maxLod = 0.0f;
    return &info;
}

IMAGE_VIEW* image_view_creation_info (VkImage image, VkFormat image_format)
{
    static IMAGE_VIEW info {};
    info.sType = get_structure_type("image view creation");
    info.image = image;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = image_format;
    info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    info.subresourceRange.baseMipLevel = 0;
    info.subresourceRange.levelCount = 1;
    info.subresourceRange.baseArrayLayer = 0;
    info.subresourceRange.layerCount = 1;
    return &info;
}

MEMORY* memory_allocation_info (uint64_t allocation_size, uint32_t memory_type_index)
{
    static MEMORY info {};
    info.sType = get_structure_type("memory allocation");
    info.memoryTypeIndex = memory_type_index;
    info.allocationSize = allocation_size;
    return &info;
}

SWAPCHAIN* swapchain_creation_info ()
{
    swapchain_support_details swap_chain_support = *get_swapchain_support_details();
    static SWAPCHAIN info {};
    info.sType = get_structure_type("swap chain creation");
    info.surface = *get_window_surface();
    info.minImageCount = *get_swapchain_image_count();
    info.imageFormat = *get_swapchain_image_format();
    info.imageColorSpace = *get_swapchain_image_colour_space();
    info.imageExtent = *get_swapchain_image_extent();
    info.imageArrayLayers = 1;
    info.imageUsage = set_image_usage_bits({ .storage = true, .colour_attachment = true });
    info.preTransform = swap_chain_support.surface_capabilities.currentTransform;
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.presentMode = *get_swapchain_image_presentation_mode();
    info.clipped = VK_TRUE;
    info.oldSwapchain = VK_NULL_HANDLE;

    uint32_t graphics_family = (*get_hardware_attributes())[ get_hardware_attribute_index() ].graphics_queue;
    uint32_t presentation_family = (*get_hardware_attributes())[ get_hardware_attribute_index() ].presentation_queue;
    uint32_t queue_family_indices[] = {graphics_family, presentation_family};

    bool graphics_equals_presentation = graphics_family == presentation_family;

    info.imageSharingMode = graphics_equals_presentation ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
    info.queueFamilyIndexCount = graphics_equals_presentation ? 0 : 2;
    info.pQueueFamilyIndices = graphics_equals_presentation ? nullptr : queue_family_indices;

    return &info;
}

SHADER* shader_module_creation_info (vector<char> code)
{
    static SHADER info {};
    info.sType = get_structure_type("shader module creation");
    info.codeSize = code.size();
    info.pCode = reinterpret_cast<const uint32_t*>( code.data() );
    return &info;
}

SHADER_STAGE* shader_stage_creation_info (VkShaderModule module)
{
    static SHADER_STAGE info {};
    info.sType = get_structure_type("shader stage creation");
    info.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    info.module = module;
    info.pName = "main";
    return &info;
}

PIPELINE_LAYOUT* pipeline_layout_creation_info (vector<VkDescriptorSetLayout> layouts)
{
    static PIPELINE_LAYOUT info {};
    info.sType = get_structure_type("pipeline layout creation");
    info.setLayoutCount = layouts.size();
    info.pSetLayouts = layouts.data();
    info.pushConstantRangeCount = 1;
    info.pPushConstantRanges = get_push_constant_range();
    return &info;
}

PIPELINE_CACHE* pipeline_cache_creation_info ()
{
    static PIPELINE_CACHE info {};
    info.sType = get_structure_type("pipeline cache creation");
    return &info;
}

PIPELINE* compute_pipeline_creation_info (SHADER_STAGE stage, VkPipelineLayout layout)
{
    static PIPELINE info {};
    info.sType = get_structure_type("compute pipeline creation");
    info.stage = stage;
    info.layout = layout;
    return &info;
}

DESCRIPTOR_LAYOUT* descriptor_layout_creation_info (vector<VkDescriptorSetLayoutBinding> bindings)
{
    static DESCRIPTOR_LAYOUT info {};
    info.sType = get_structure_type("descriptor set layout creation");
    info.bindingCount = bindings.size();
    info.pBindings = bindings.data();
    return &info;
}

DESCRIPTOR_POOL* descriptor_pool_creation_info (VkDescriptorPoolSize* pool_size)
{
    static DESCRIPTOR_POOL info {};
    info.sType = get_structure_type("descriptor pool creation");
    info.maxSets = pool_size->descriptorCount;
    info.poolSizeCount = 1;
    info.pPoolSizes = pool_size;
    return &info;
}

DESCRIPTOR_SET* descriptor_set_allocation_info (VkDescriptorPool pool, VkDescriptorSetLayout* layout)
{
    static DESCRIPTOR_SET info {};
    info.sType = get_structure_type("descriptor set allocation");
    info.descriptorPool = pool;
    info.descriptorSetCount = 1;
    info.pSetLayouts = layout;
    return &info;
}

COMMAND_POOL* command_pool_creation_info ()
{
    static COMMAND_POOL info {};
    info.sType = get_structure_type("command pool creation");
    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info.queueFamilyIndex = (*get_hardware_attributes())[ get_hardware_attribute_index() ].compute_queue;
    return &info;
}

COMMAND_BUFFER* command_buffer_allocation_info (uint32_t buffer_count)
{
    static COMMAND_BUFFER info{};
    info.sType = get_structure_type("command buffer allocation");
    info.commandPool = *get_command_pool();
    info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    info.commandBufferCount = buffer_count;
    return &info;
}

COMMAND_BUFFER_BEGIN* command_buffer_begin_info ()
{
    static COMMAND_BUFFER_BEGIN info {};
    info.sType = get_structure_type("command buffer begin");
    info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    info.pInheritanceInfo = nullptr;
    return &info;
}

SEMAPHORE* semaphore_creation_info ()
{
    static SEMAPHORE info {};
    info.sType = get_structure_type("semaphore");
    return &info;
}

FENCE* fence_creation_info ()
{
    static FENCE info {};
    info.sType = get_structure_type("fence");
    info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    return &info;
}

SUBMIT* submit_info (int swapchain_frame)
{
    static VkPipelineStageFlags pipeline_stages[] = { VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT };
    static SUBMIT info {};
    info.sType = get_structure_type("submit");
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = get_gpu_image_available(swapchain_frame);
    info.pWaitDstStageMask = pipeline_stages;
    info.commandBufferCount = 1;
    info.pCommandBuffers = get_command_buffer(swapchain_frame);
    info.signalSemaphoreCount = 1;
    info.pSignalSemaphores = get_gpu_image_rendered(swapchain_frame);
    return &info;
}

PRESENT* present_info (uint32_t swapchain_frame, uint32_t* swapchain_image_index)
{
    static PRESENT info {};
    info.sType = get_structure_type("present");
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = get_gpu_image_rendered(swapchain_frame);
    info.swapchainCount = 1;
    info.pSwapchains = get_swapchain();
    info.pImageIndices = swapchain_image_index;
    return &info;
}
