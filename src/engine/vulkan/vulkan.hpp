// INCLUDE
#include "../validation/validation.hpp"
#include "../compact/compact.hpp"
#include "../structure/structure.hpp"

// CODE
void initialize_vulkan ();
void initialize_instance_extensions ();
void cleanup_vulkan ();

// ACCESS
VkInstance* get_vulkan_instance ();
void set_vulkan_instance (VkInstance instance);
vector<const char*>* get_vulkan_extensions ();
void set_vulkan_extensions (vector<const char*> extensions);
