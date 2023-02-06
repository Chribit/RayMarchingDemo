// DEFINE
#if FOR_WIN
    #define PATH_SEPERATOR "\\"
#else
    #define PATH_SEPERATOR "/"
#endif
#define GLFW_INCLUDE_VULKAN 


// INCLUDE
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "cstring"
#include "chrono"
#include "iostream"
#include "vector"
#include "fstream"
#include "set"
#include "optional"
#include "cstdint"
#include "limits"
#include "algorithm"
#include "math.h"
#include "iomanip"
#include "functional"
#include "numeric"


// OS SPECIFIC INCLUDE
#if FOR_MAC
    #include "mach-o/dyld.h"
#elif FOR_WIN
    #include "libloaderapi.h"
#elif FOR_LIN
    #include "unistd.h"
#endif


// NAMESPACE
using namespace std;
using namespace std::chrono;


// DATA
#ifndef DEFINED_AABB
#define DEFINED_AABB
struct AABB
{
    glm::vec2 lower_bound;
    glm::vec2 upper_bound;
};
#endif


// CODE
void replace_characters (string& text, string original, string replacement);
string get_executable_path ();
string get_working_directory ();
string get_path (string relative_path);
vector<char> read_file (string& file_path);
void throw_error (bool condition, string error_text);
string get_result_string (VkResult vulkan_result);
VkStructureType get_structure_type (string type);
string get_format_string (VkFormat format);
string get_colour_space_string (VkColorSpaceKHR colour_space);
string get_presentation_mode_string (VkPresentModeKHR presentation_mode);
glm::vec2 calculate_dimensions (glm::vec2 lower_bound, glm::vec2 upper_bound);
