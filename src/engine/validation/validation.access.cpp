// INCLUDE
#include "./validation.hpp"

// DATA
static struct {
    uint32_t available_layer_count;
    vector<VkLayerProperties> available_layers;
    vector<char*> layers = {
        (char*) "VK_LAYER_KHRONOS_validation"
    };
} validation;

// CODE
uint32_t* get_available_layer_count ()
{
    return &validation.available_layer_count;
}

void set_available_layer_count (uint32_t count)
{
    validation.available_layer_count = count;
}

vector<VkLayerProperties>* get_available_validation_layers ()
{
    return &validation.available_layers;
}

void set_available_validation_layers (vector<VkLayerProperties> layers)
{
    validation.available_layers = layers;
}

vector<char*>* get_validation_layers ()
{
    return &validation.layers;
}

void set_validation_layers (vector<char*> layers)
{
    validation.layers = layers;
}

int32_t get_validation_layer_count ()
{
    vector<char*> layers = *get_validation_layers();
    return layers.size();
}
