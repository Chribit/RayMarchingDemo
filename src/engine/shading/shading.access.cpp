// INCLUDE
#include "./shading.hpp"

// DATA
static struct {
    VkPipeline pipeline;
    VkPipelineLayout pipeline_layout;
    VkPipelineCache pipeline_cache;
} shading;

// CODE
VkPipeline* get_shading_pipeline ()
{
    return &shading.pipeline;
}

void get_shading_pipeline (VkPipeline pipeline)
{
    shading.pipeline = pipeline;
}

VkPipelineLayout* get_shading_pipeline_layout ()
{
    return &shading.pipeline_layout;
}

void set_shading_pipeline_layout (VkPipelineLayout layout)
{
    shading.pipeline_layout = layout;
}

VkPipelineCache* get_shading_pipeline_cache ()
{
    return &shading.pipeline_cache;
}

void set_shading_pipeline_cache (VkPipelineCache cache)
{
    shading.pipeline_cache = cache;
}
