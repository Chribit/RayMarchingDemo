// INCLUDE
#include "./shading.hpp"

// CODE
void initialize_shaders ()
{
    vector<VkDescriptorSetLayout> layouts = { *get_descriptor_buffer_layout(), *get_descriptor_image_layout() };
    VkResult result = vkCreatePipelineLayout(
        *get_hardware(),
        pipeline_layout_creation_info(layouts),
        nullptr,
        get_shading_pipeline_layout()
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of pipeline layout failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );

    result = vkCreatePipelineCache( *get_hardware(), pipeline_cache_creation_info(), nullptr, get_shading_pipeline_cache() );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of pipeline cache failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );

    VkShaderModule test_module = create_shader_module("shading");

    result = vkCreateComputePipelines(
        *get_hardware(),
        *get_shading_pipeline_cache(),
        1,
        compute_pipeline_creation_info( *shader_stage_creation_info( test_module ), *get_shading_pipeline_layout() ),
        nullptr,
        get_shading_pipeline()
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of pipeline failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );

    vkDestroyShaderModule( *get_hardware(), test_module, nullptr);
}

VkShaderModule create_shader_module (string file_name)
{
    string compute_shader_path = get_absolute_path("shading/" + file_name + ".spv");

    string file_content = read_file(compute_shader_path);
    vector<char> compute_shader_code(file_content.begin(), file_content.end());

    VkShaderModule shader_module;
    VkResult result = vkCreateShaderModule( *get_hardware(), shader_module_creation_info(compute_shader_code), nullptr, &shader_module);

    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of shader module failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );

    return shader_module;
}

uint32_t determine_workgroup_count (int pixels)
{
    float fractional_count = (float) pixels / 16.0;
    return ceilf( fractional_count );
}

uint32_t set_shader_stage_bits (shader_stage_bits given_bits)
{
    uint32_t bits = 0;
    if (given_bits.vertex) bits += 1;
    if (given_bits.tessellation_control) bits += 2;
    if (given_bits.tessellation_evaluation) bits += 4;
    if (given_bits.geometry) bits += 8;
    if (given_bits.fragment) bits += 16;
    if (given_bits.compute) bits += 32;
    return bits;
}

void cleanup_shaders ()
{
    vkDestroyPipelineCache( *get_hardware(), *get_shading_pipeline_cache(), nullptr );
    vkDestroyPipelineLayout( *get_hardware(), *get_shading_pipeline_layout(), nullptr);
    vkDestroyPipeline( *get_hardware(), *get_shading_pipeline(), nullptr );
}
