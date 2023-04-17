// DEFINE
#ifndef VERSION
    #define VERSION "0.0.0"
#endif

// INCLUDE
#include "../compact/compact.hpp"
#include "../hardware/hardware.hpp"
#include "../descriptor/descriptor.hpp"
#include "../file/file.hpp"

// CODE
void initialize_shaders ();
VkShaderModule create_shader_module (string file_name);
uint32_t determine_workgroup_count (int pixels);
uint32_t set_shader_stage_bits (shader_stage_bits given_bits);
void cleanup_shaders ();

// ACCESS
VkPipeline* get_shading_pipeline ();
void get_shading_pipeline (VkPipeline pipeline);
VkPipelineLayout* get_shading_pipeline_layout ();
void set_shading_pipeline_layout (VkPipelineLayout layout);
VkPipelineCache* get_shading_pipeline_cache ();
void set_shading_pipeline_cache (VkPipelineCache cache);
