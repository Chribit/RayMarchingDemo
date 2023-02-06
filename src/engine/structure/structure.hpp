// DEFINE
#define APPLICATION VkApplicationInfo
#define INSTANCE VkInstanceCreateInfo
#define QUEUE VkDeviceQueueCreateInfo
#define DEVICE VkDeviceCreateInfo
#define BUFFER VkBufferCreateInfo
#define IMAGE VkImageCreateInfo
#define IMAGE_SAMPLER VkSamplerCreateInfo
#define IMAGE_VIEW VkImageViewCreateInfo
#define MEMORY VkMemoryAllocateInfo
#define SWAPCHAIN VkSwapchainCreateInfoKHR
#define SHADER VkShaderModuleCreateInfo
#define SHADER_STAGE VkPipelineShaderStageCreateInfo
#define PIPELINE_LAYOUT VkPipelineLayoutCreateInfo
#define PIPELINE_CACHE VkPipelineCacheCreateInfo
#define PIPELINE VkComputePipelineCreateInfo
#define DESCRIPTOR_LAYOUT VkDescriptorSetLayoutCreateInfo
#define DESCRIPTOR_POOL VkDescriptorPoolCreateInfo
#define DESCRIPTOR_SET VkDescriptorSetAllocateInfo
#define COMMAND_POOL VkCommandPoolCreateInfo
#define COMMAND_BUFFER VkCommandBufferAllocateInfo
#define COMMAND_BUFFER_BEGIN VkCommandBufferBeginInfo
#define SEMAPHORE VkSemaphoreCreateInfo
#define FENCE VkFenceCreateInfo
#define SUBMIT VkSubmitInfo
#define PRESENT VkPresentInfoKHR

// INCLUDE
#include "../compact/compact.hpp"
#include "../validation/validation.hpp"

// CODE
APPLICATION* application_info ();
INSTANCE* instance_creation_info ();
QUEUE* queue_creation_info (uint32_t queue_family);
DEVICE* device_creation_info (vector<VkDeviceQueueCreateInfo>& queue_creation_infos);
BUFFER* buffer_creation_info (uint64_t buffer_size, uint32_t usage_flags, bool exclusive);
IMAGE* image_creation_info (VkExtent2D extent);
IMAGE_SAMPLER* image_sampler_creation_info (bool normalized_coordinates);
IMAGE_VIEW* image_view_creation_info (VkImage image, VkFormat image_format);
MEMORY* memory_allocation_info  (uint64_t allocation_size, uint32_t memory_type_index);
SWAPCHAIN* swapchain_creation_info ();
SHADER* shader_module_creation_info (vector<char> code);
SHADER_STAGE* shader_stage_creation_info (VkShaderModule module);
PIPELINE_LAYOUT* pipeline_layout_creation_info (vector<VkDescriptorSetLayout> layouts);
PIPELINE_CACHE* pipeline_cache_creation_info ();
PIPELINE* compute_pipeline_creation_info (SHADER_STAGE stage, VkPipelineLayout layout);
DESCRIPTOR_LAYOUT* descriptor_layout_creation_info (vector<VkDescriptorSetLayoutBinding> bindings);
DESCRIPTOR_POOL* descriptor_pool_creation_info (VkDescriptorPoolSize* pool_size);
DESCRIPTOR_SET* descriptor_set_allocation_info (VkDescriptorPool pool, VkDescriptorSetLayout* layout);
COMMAND_POOL* command_pool_creation_info ();
COMMAND_BUFFER* command_buffer_allocation_info (uint32_t buffer_count);
COMMAND_BUFFER_BEGIN* command_buffer_begin_info ();
SEMAPHORE* semaphore_creation_info ();
FENCE* fence_creation_info ();
SUBMIT* submit_info (int swapchain_frame);
PRESENT* present_info (uint32_t swapchain_frame, uint32_t* swapchain_image_index);
