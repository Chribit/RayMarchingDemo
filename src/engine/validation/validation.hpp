// DEFINE
#if DEBUG
    #define USE_VALIDATION_LAYERS true
#else
    #define USE_VALIDATION_LAYERS false
#endif

// INCLUDE
#include "../compact/compact.hpp"

// CODE
void initialize_validation_layers ();
void initialize_available_validation_layers ();

// ACCESS
uint32_t* get_available_layer_count ();
void set_available_layer_count (uint32_t count);
vector<VkLayerProperties>* get_available_validation_layers ();
void set_available_validation_layers (vector<VkLayerProperties> layers);
vector<char*>* get_validation_layers ();
void set_validation_layers (vector<char*> layers);
int32_t get_validation_layer_count ();
