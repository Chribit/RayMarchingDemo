// INCLUDE
#include "./compact.hpp"


// CODE
void replace_characters (string& text, string original, string replacement)
{
    size_t position;
    while ((position = text.find(original)) != string::npos)
    {
        text.replace(position, 1, replacement);
    }
}

string get_executable_path ()
{
    char buffer[PATH_MAX] = {0};
    uint32_t buffer_length = PATH_MAX;
    bool success = false;
    string os;

    #if FOR_MAC
        success = int(_NSGetExecutablePath(buffer, &buffer_length)) == 0;
        os = "MAC";
    #elif FOR_WIN
        success = int(GetModuleFileName(NULL, buffer, buffer_length)) != 0;
        os = "WINDOWS";
    #elif FOR_LIN
        success = int(readlink("/proc/self/exe", buffer, buffer_length)) != -1;
        os = "LINUX";
    #endif

    throw_error(!success, 
        "\n"
        "ERROR: Retrieval of path to executable was unsuccessful.\n"
        "       Executed get_absolute_path() on " + os + ".\n"
        "       The resulting buffer contains:\n"
        "       " + buffer
    );

    return string(buffer, buffer_length);
}

string get_working_directory ()
{
    string path = get_executable_path();
    path = path.substr(0, path.find_last_of( PATH_SEPERATOR ));
    return path;
}

string get_path (string relative_path)
{
    #if FOR_WIN
        replace_characters(relative_path, "/", "\\");
    #else
        replace_characters(relative_path, "\\", "/");
    #endif

    return get_working_directory() + PATH_SEPERATOR + relative_path;
}

vector<char> read_file (string& file_path)
{
    ifstream file(file_path, ios::ate | ios::binary);

    throw_error(!file.is_open(), 
        "\n"
        "ERROR: Opening file '" + file_path + "' failed.\n"
    );

    size_t file_size = (size_t) file.tellg();

    vector<char> buffer(file_size);

    file.seekg(0);
    file.read(buffer.data(), file_size);
    file.close();

    return buffer;
}

void throw_error (bool condition, string error_text)
{
    if (condition) throw runtime_error(error_text);
}

string get_result_string (VkResult vulkan_result)
{
    switch (vulkan_result)
    {
        case VK_SUCCESS:
            return "SUCCESS";
        case VK_NOT_READY:
            return "NOT READY";
        case VK_TIMEOUT:
            return "TIMEOUT";
        case VK_EVENT_SET:
            return "EVENT SET";
        case VK_EVENT_RESET:
            return "EVENT RESET";
        case VK_INCOMPLETE:
            return "INCOMPLETE";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "OUT OF HOST MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "OUT OF DEVICE MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "INITIALIZATION FAILED";
        case VK_ERROR_DEVICE_LOST:
            return "DEVICE LOST";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "MEMORY MAP FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "LAYER NOT PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "EXTENSION NOT PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "FEATURE NOT PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "INCOMPATIBLE DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "TOO MANY OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "FORMAT NOT SUPPORTED";
        case VK_ERROR_FRAGMENTED_POOL:
            return "FRAGMENTED POOL";
        case VK_ERROR_UNKNOWN:
            return "UNKNOWN ERROR";
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            return "OUT OF POOL MEMORY";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "OUT OF DATE";
        case VK_ERROR_INVALID_SHADER_NV:
            return "INVALID SHADER";
        case VK_PIPELINE_COMPILE_REQUIRED_EXT:
            return "PIPELINE COMPILATION REQUIRES EXTENSION";
        case VK_SUBOPTIMAL_KHR:
            return "SUBOPTIMAL";
        default:
            return "UNKNOWN VULKAN RESULT NR. " + to_string(vulkan_result);
    }
}

VkStructureType get_structure_type (string type)
{
    if (type == "application") return VK_STRUCTURE_TYPE_APPLICATION_INFO;
    else if (type == "instance creation") return VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    else if (type == "queue creation") return VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    else if (type == "device creation") return VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    else if (type == "buffer creation") return VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    else if (type == "image creation") return VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    else if (type == "image sampler creation") return VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    else if (type == "image view creation") return VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    else if (type == "memory allocation") return VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    else if (type == "swap chain creation") return VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    else if (type == "frame buffer creation") return VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    else if (type == "shader module creation") return VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    else if (type == "shader stage creation") return VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    else if (type == "pipeline layout creation") return VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    else if (type == "pipeline cache creation") return VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    else if (type == "compute pipeline creation") return VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    else if (type == "descriptor set layout creation") return VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    else if (type == "descriptor pool creation") return VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    else if (type == "descriptor set allocation") return VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    else if (type == "command pool creation") return VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    else if (type == "command buffer allocation") return VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    else if (type == "command buffer begin") return VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    else if (type == "render pass begin") return VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    else if (type == "semaphore") return VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    else if (type == "fence") return VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    else if (type == "submit") return VK_STRUCTURE_TYPE_SUBMIT_INFO;
    else if (type == "present") return VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    else
    {
        throw_error(true,
                "\n"
                "ERROR: Structure type retrieval failed.\n"
                "       The provided vulkan struct type '" + type + "' does not exist.\n"
                "       Returning type = 0 (vulkan application info type) as a default.\n"
                "       Valid struct types are:\n"
            );
        return VK_STRUCTURE_TYPE_APPLICATION_INFO;
    }
}

string get_format_string (VkFormat format)
{
    switch (format)
    {
        case VK_FORMAT_UNDEFINED:
            return "UNDEFINED";
        case VK_FORMAT_R4G4_UNORM_PACK8:
            return "R4G4_UNORM_PACK8";
        case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
            return "R4G4B4A4_UNORM_PACK16";
        case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
            return "B4G4R4A4_UNORM_PACK16";
        case VK_FORMAT_R5G6B5_UNORM_PACK16:
            return "R5G6B5_UNORM_PACK16";
        case VK_FORMAT_B5G6R5_UNORM_PACK16:
            return "B5G6R5_UNORM_PACK16";
        case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
            return "R5G5B5A1_UNORM_PACK16";
        case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
            return "B5G5R5A1_UNORM_PACK16";
        case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
            return "A1R5G5B5_UNORM_PACK16";
        case VK_FORMAT_R8_UNORM:
            return "R8_UNORM";
        case VK_FORMAT_R8_SNORM:
            return "R8_SNORM";
        case VK_FORMAT_R8_USCALED:
            return "R8_USCALED";
        case VK_FORMAT_R8_SSCALED:
            return "R8_SSCALED";
        case VK_FORMAT_R8_UINT:
            return "R8_UINT";
        case VK_FORMAT_R8_SINT:
            return "R8_SINT";
        case VK_FORMAT_R8_SRGB:
            return "R8_SRGB";
        case VK_FORMAT_R8G8_UNORM:
            return "R8G8_UNORM";
        case VK_FORMAT_R8G8_SNORM:
            return "R8G8_SNORM";
        case VK_FORMAT_R8G8_USCALED:
            return "R8G8_USCALED";
        case VK_FORMAT_R8G8_SSCALED:
            return "R8G8_SSCALED";
        case VK_FORMAT_R8G8_UINT:
            return "R8G8_UINT";
        case VK_FORMAT_R8G8_SINT:
            return "R8G8_SINT";
        case VK_FORMAT_R8G8_SRGB:
            return "R8G8_SRGB";
        case VK_FORMAT_R8G8B8_UNORM:
            return "R8G8B8_UNORM";
        case VK_FORMAT_R8G8B8_SNORM:
            return "R8G8B8_SNORM";
        case VK_FORMAT_R8G8B8_USCALED:
            return "R8G8B8_USCALED";
        case VK_FORMAT_R8G8B8_SSCALED:
            return "R8G8B8_SSCALED";
        case VK_FORMAT_R8G8B8_UINT:
            return "R8G8B8_UINT";
        case VK_FORMAT_R8G8B8_SINT:
            return "R8G8B8_SINT";
        case VK_FORMAT_R8G8B8_SRGB:
            return "R8G8B8_SRGB";
        case VK_FORMAT_B8G8R8_UNORM:
            return "B8G8R8_UNORM";
        case VK_FORMAT_B8G8R8_SNORM:
            return "B8G8R8_SNORM";
        case VK_FORMAT_B8G8R8_USCALED:
            return "B8G8R8_USCALED";
        case VK_FORMAT_B8G8R8_SSCALED:
            return "B8G8R8_SSCALED";
        case VK_FORMAT_B8G8R8_UINT:
            return "B8G8R8_UINT";
        case VK_FORMAT_B8G8R8_SINT:
            return "B8G8R8_SINT";
        case VK_FORMAT_B8G8R8_SRGB:
            return "B8G8R8_SRGB";
        case VK_FORMAT_R8G8B8A8_UNORM:
            return "R8G8B8A8_UNORM";
        case VK_FORMAT_R8G8B8A8_SNORM:
            return "R8G8B8A8_SNORM";
        case VK_FORMAT_R8G8B8A8_USCALED:
            return "R8G8B8A8_USCALED";
        case VK_FORMAT_R8G8B8A8_SSCALED:
            return "R8G8B8A8_SSCALED";
        case VK_FORMAT_R8G8B8A8_UINT:
            return "R8G8B8A8_UINT";
        case VK_FORMAT_R8G8B8A8_SINT:
            return "R8G8B8A8_SINT";
        case VK_FORMAT_R8G8B8A8_SRGB:
            return "R8G8B8A8_SRGB";
        case VK_FORMAT_B8G8R8A8_UNORM:
            return "B8G8R8A8_UNORM";
        case VK_FORMAT_B8G8R8A8_SNORM:
            return "B8G8R8A8_SNORM";
        case VK_FORMAT_B8G8R8A8_USCALED:
            return "B8G8R8A8_USCALED";
        case VK_FORMAT_B8G8R8A8_SSCALED:
            return "B8G8R8A8_SSCALED";
        case VK_FORMAT_B8G8R8A8_UINT:
            return "B8G8R8A8_UINT";
        case VK_FORMAT_B8G8R8A8_SINT:
            return "B8G8R8A8_SINT";
        case VK_FORMAT_B8G8R8A8_SRGB:
            return "B8G8R8A8_SRGB";
        case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
            return "A8B8G8R8_UNORM_PACK32";
        case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
            return "A8B8G8R8_SNORM_PACK32";
        case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
            return "A8B8G8R8_USCALED_PACK32";
        case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
            return "A8B8G8R8_SSCALED_PACK32";
        case VK_FORMAT_A8B8G8R8_UINT_PACK32:
            return "A8B8G8R8_UINT_PACK32";
        case VK_FORMAT_A8B8G8R8_SINT_PACK32:
            return "A8B8G8R8_SINT_PACK32";
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
            return "A8B8G8R8_SRGB_PACK32";
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
            return "A2R10G10B10_UNORM_PACK32";
        case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
            return "A2R10G10B10_SNORM_PACK32";
        case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
            return "A2R10G10B10_USCALED_PACK32";
        case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
            return "A2R10G10B10_SSCALED_PACK32";
        case VK_FORMAT_A2R10G10B10_UINT_PACK32:
            return "A2R10G10B10_UINT_PACK32";
        case VK_FORMAT_A2R10G10B10_SINT_PACK32:
            return "A2R10G10B10_SINT_PACK32";
        case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
            return "A2B10G10R10_UNORM_PACK32";
        case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
            return "A2B10G10R10_SNORM_PACK32";
        case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
            return "A2B10G10R10_USCALED_PACK32";
        case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
            return "A2B10G10R10_SSCALED_PACK32";
        case VK_FORMAT_A2B10G10R10_UINT_PACK32:
            return "A2B10G10R10_UINT_PACK32";
        case VK_FORMAT_A2B10G10R10_SINT_PACK32:
            return "A2B10G10R10_SINT_PACK32";
        case VK_FORMAT_R16_UNORM:
            return "R16_UNORM";
        case VK_FORMAT_R16_SNORM:
            return "R16_SNORM";
        case VK_FORMAT_R16_USCALED:
            return "R16_USCALED";
        case VK_FORMAT_R16_SSCALED:
            return "R16_SSCALED";
        case VK_FORMAT_R16_UINT:
            return "R16_UINT";
        case VK_FORMAT_R16_SINT:
            return "R16_SINT";
        case VK_FORMAT_R16_SFLOAT:
            return "R16_SFLOAT";
        case VK_FORMAT_R16G16_UNORM:
            return "R16G16_UNORM";
        case VK_FORMAT_R16G16_SNORM:
            return "R16G16_SNORM";
        case VK_FORMAT_R16G16_USCALED:
            return "R16G16_USCALED";
        case VK_FORMAT_R16G16_SSCALED:
            return "R16G16_SSCALED";
        case VK_FORMAT_R16G16_UINT:
            return "R16G16_UINT";
        case VK_FORMAT_R16G16_SINT:
            return "R16G16_SINT";
        case VK_FORMAT_R16G16_SFLOAT:
            return "R16G16_SFLOAT";
        case VK_FORMAT_R16G16B16_UNORM:
            return "R16G16B16_UNORM";
        case VK_FORMAT_R16G16B16_SNORM:
            return "R16G16B16_SNORM";
        case VK_FORMAT_R16G16B16_USCALED:
            return "R16G16B16_USCALED";
        case VK_FORMAT_R16G16B16_SSCALED:
            return "R16G16B16_SSCALED";
        case VK_FORMAT_R16G16B16_UINT:
            return "R16G16B16_UINT";
        case VK_FORMAT_R16G16B16_SINT:
            return "R16G16B16_SINT";
        case VK_FORMAT_R16G16B16_SFLOAT:
            return "R16G16B16_SFLOAT";
        case VK_FORMAT_R16G16B16A16_UNORM:
            return "R16G16B16A16_UNORM";
        case VK_FORMAT_R16G16B16A16_SNORM:
            return "R16G16B16A16_SNORM";
        case VK_FORMAT_R16G16B16A16_USCALED:
            return "R16G16B16A16_USCALED";
        case VK_FORMAT_R16G16B16A16_SSCALED:
            return "R16G16B16A16_SSCALED";
        case VK_FORMAT_R16G16B16A16_UINT:
            return "R16G16B16A16_UINT";
        case VK_FORMAT_R16G16B16A16_SINT:
            return "R16G16B16A16_SINT";
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            return "R16G16B16A16_SFLOAT";
        case VK_FORMAT_R32_UINT:
            return "R32_UINT";
        case VK_FORMAT_R32_SINT:
            return "R32_SINT";
        case VK_FORMAT_R32_SFLOAT:
            return "R32_SFLOAT";
        case VK_FORMAT_R32G32_UINT:
            return "R32G32_UINT";
        case VK_FORMAT_R32G32_SINT:
            return "R32G32_SINT";
        case VK_FORMAT_R32G32_SFLOAT:
            return "R32G32_SFLOAT";
        case VK_FORMAT_R32G32B32_UINT:
            return "R32G32B32_UINT";
        case VK_FORMAT_R32G32B32_SINT:
            return "R32G32B32_SINT";
        case VK_FORMAT_R32G32B32_SFLOAT:
            return "R32G32B32_SFLOAT";
        case VK_FORMAT_R32G32B32A32_UINT:
            return "R32G32B32A32_UINT";
        case VK_FORMAT_R32G32B32A32_SINT:
            return "R32G32B32A32_SINT";
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            return "R32G32B32A32_SFLOAT";
        case VK_FORMAT_R64_UINT:
            return "R64_UINT";
        case VK_FORMAT_R64_SINT:
            return "R64_SINT";
        case VK_FORMAT_R64_SFLOAT:
            return "R64_SFLOAT";
        case VK_FORMAT_R64G64_UINT:
            return "R64G64_UINT";
        case VK_FORMAT_R64G64_SINT:
            return "R64G64_SINT";
        case VK_FORMAT_R64G64_SFLOAT:
            return "R64G64_SFLOAT";
        case VK_FORMAT_R64G64B64_UINT:
            return "R64G64B64_UINT";
        case VK_FORMAT_R64G64B64_SINT:
            return "R64G64B64_SINT";
        case VK_FORMAT_R64G64B64_SFLOAT:
            return "R64G64B64_SFLOAT";
        case VK_FORMAT_R64G64B64A64_UINT:
            return "R64G64B64A64_UINT";
        case VK_FORMAT_R64G64B64A64_SINT:
            return "R64G64B64A64_SINT";
        case VK_FORMAT_R64G64B64A64_SFLOAT:
            return "R64G64B64A64_SFLOAT";
        case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
            return "B10G11R11_UFLOAT_PACK32";
        case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
            return "E5B9G9R9_UFLOAT_PACK32";
        case VK_FORMAT_D16_UNORM:
            return "D16_UNORM";
        case VK_FORMAT_X8_D24_UNORM_PACK32:
            return "X8_D24_UNORM_PACK32";
        case VK_FORMAT_D32_SFLOAT:
            return "D32_SFLOAT";
        case VK_FORMAT_S8_UINT:
            return "S8_UINT";
        case VK_FORMAT_D16_UNORM_S8_UINT:
            return "D16_UNORM_S8_UINT";
        case VK_FORMAT_D24_UNORM_S8_UINT:
            return "D24_UNORM_S8_UINT";
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            return "D32_SFLOAT_S8_UINT";
        case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
            return "BC1_RGB_UNORM_BLOCK";
        case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
            return "BC1_RGB_SRGB_BLOCK";
        case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
            return "BC1_RGBA_UNORM_BLOCK";
        case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
            return "BC1_RGBA_SRGB_BLOCK";
        case VK_FORMAT_BC2_UNORM_BLOCK:
            return "BC2_UNORM_BLOCK";
        case VK_FORMAT_BC2_SRGB_BLOCK:
            return "BC2_SRGB_BLOCK";
        case VK_FORMAT_BC3_UNORM_BLOCK:
            return "BC3_UNORM_BLOCK";
        case VK_FORMAT_BC3_SRGB_BLOCK:
            return "BC3_SRGB_BLOCK";
        case VK_FORMAT_BC4_UNORM_BLOCK:
            return "BC4_UNORM_BLOCK";
        case VK_FORMAT_BC4_SNORM_BLOCK:
            return "BC4_SNORM_BLOCK";
        case VK_FORMAT_BC5_UNORM_BLOCK:
            return "BC5_UNORM_BLOCK";
        case VK_FORMAT_BC5_SNORM_BLOCK:
            return "BC5_SNORM_BLOCK";
        case VK_FORMAT_BC6H_UFLOAT_BLOCK:
            return "BC6H_UFLOAT_BLOCK";
        case VK_FORMAT_BC6H_SFLOAT_BLOCK:
            return "BC6H_SFLOAT_BLOCK";
        case VK_FORMAT_BC7_UNORM_BLOCK:
            return "BC7_UNORM_BLOCK";
        case VK_FORMAT_BC7_SRGB_BLOCK:
            return "BC7_SRGB_BLOCK";
        case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
            return "ETC2_R8G8B8_UNORM_BLOCK";
        case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
            return "ETC2_R8G8B8_SRGB_BLOCK";
        case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
            return "ETC2_R8G8B8A1_UNORM_BLOCK";
        case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
            return "ETC2_R8G8B8A1_SRGB_BLOCK";
        case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
            return "ETC2_R8G8B8A8_UNORM_BLOCK";
        case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
            return "ETC2_R8G8B8A8_SRGB_BLOCK";
        case VK_FORMAT_EAC_R11_UNORM_BLOCK:
            return "EAC_R11_UNORM_BLOCK";
        case VK_FORMAT_EAC_R11_SNORM_BLOCK:
            return "EAC_R11_SNORM_BLOCK";
        case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
            return "EAC_R11G11_UNORM_BLOCK";
        case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
            return "EAC_R11G11_SNORM_BLOCK";
        case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
            return "ASTC_4x4_UNORM_BLOCK";
        case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
            return "ASTC_4x4_SRGB_BLOCK";
        case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
            return "ASTC_5x4_UNORM_BLOCK";
        case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
            return "ASTC_5x4_SRGB_BLOCK";
        case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
            return "ASTC_5x5_UNORM_BLOCK";
        case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
            return "ASTC_5x5_SRGB_BLOCK";
        case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
            return "ASTC_6x5_UNORM_BLOCK";
        case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
            return "ASTC_6x5_SRGB_BLOCK";
        case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
            return "ASTC_6x6_UNORM_BLOCK";
        case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
            return "ASTC_6x6_SRGB_BLOCK";
        case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
            return "ASTC_8x5_UNORM_BLOCK";
        case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
            return "ASTC_8x5_SRGB_BLOCK";
        case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
            return "ASTC_8x6_UNORM_BLOCK";
        case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
            return "ASTC_8x6_SRGB_BLOCK";
        case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
            return "ASTC_8x8_UNORM_BLOCK";
        case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
            return "ASTC_8x8_SRGB_BLOCK";
        case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
            return "ASTC_10x5_UNORM_BLOCK";
        case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
            return "ASTC_10x5_SRGB_BLOCK";
        case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
            return "ASTC_10x6_UNORM_BLOCK";
        case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
            return "ASTC_10x6_SRGB_BLOCK";
        case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
            return "ASTC_10x8_UNORM_BLOCK";
        case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
            return "ASTC_10x8_SRGB_BLOCK";
        case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
            return "ASTC_10x10_UNORM_BLOCK";
        case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
            return "ASTC_10x10_SRGB_BLOCK";
        case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
            return "ASTC_12x10_UNORM_BLOCK";
        case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
            return "ASTC_12x10_SRGB_BLOCK";
        case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
            return "ASTC_12x12_UNORM_BLOCK";
        case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
            return "ASTC_12x12_SRGB_BLOCK";
        default:
            return "UNKNOWN VULKAN SURFACE FORMAT";
    }
}

string get_colour_space_string (VkColorSpaceKHR colour_space)
{
    switch (colour_space)
    {
        case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:
            return "SRGB_NONLINEAR";
        case VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT:
            return "DISPLAY_P3_NONLINEAR";
        case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT:
            return "EXTENDED_SRGB_LINEAR";
        case VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT:
            return "DISPLAY_P3_LINEAR";
        case VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT:
            return "DCI_P3_NONLINEAR";
        case VK_COLOR_SPACE_BT709_LINEAR_EXT:
            return "BT709_LINEAR";
        case VK_COLOR_SPACE_BT709_NONLINEAR_EXT:
            return "BT709_NONLINEAR";
        case VK_COLOR_SPACE_BT2020_LINEAR_EXT:
            return "BT2020_LINEAR";
        case VK_COLOR_SPACE_HDR10_ST2084_EXT:
            return "HDR10_ST2084";
        case VK_COLOR_SPACE_DOLBYVISION_EXT:
            return "DOLBYVISION";
        case VK_COLOR_SPACE_HDR10_HLG_EXT:
            return "HDR10_HLG";
        case VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT:
            return "ADOBERGB_LINEAR";
        case VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT:
            return "ADOBERGB_NONLINEAR";
        case VK_COLOR_SPACE_PASS_THROUGH_EXT:
            return "PASS_THROUGH";
        case VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT:
            return "EXTENDED_SRGB_NONLINEAR";
        case VK_COLOR_SPACE_DISPLAY_NATIVE_AMD:
            return "DISPLAY_NATIVE";
        default:
            return "UNKNOWN VULKAN COLOUR SPACE";
    }
}

string get_presentation_mode_string (VkPresentModeKHR presentation_mode)
{
    switch (presentation_mode)
    {
        case VK_PRESENT_MODE_IMMEDIATE_KHR:
            return "IMMEDIATE";
        case VK_PRESENT_MODE_MAILBOX_KHR:
            return "MAILBOX";
        case VK_PRESENT_MODE_FIFO_KHR:
            return "FIFO";
        case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
            return "FIFO RELAXED";
        case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
            return "SHARED DEMAND REFRESH";
        case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
            return "SHARED CONTINUOUS REFRESH";
        default:
            return "UNKNOWN VULKAN PRESENTATION MODE";
    } 
}

glm::vec2 calculate_dimensions (glm::vec2 lower_bound, glm::vec2 upper_bound)
{
    return upper_bound - lower_bound;
}
