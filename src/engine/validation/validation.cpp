// INCLUDE
#include "./validation.hpp"

// CODE
void initialize_validation_layers ()
{   
    initialize_available_validation_layers();
    vector<VkLayerProperties> available_layers = *get_available_validation_layers();

    for (char* layer_name : *get_validation_layers())
    {
        bool layer_found = false;

        for (const auto& layer_properties : *get_available_validation_layers())
        {
            if (strcmp(layer_name, layer_properties.layerName) == 0)
            {
                layer_found = true;
                break;
            }
        }

        throw_error(!layer_found && USE_VALIDATION_LAYERS, 
            "\n"
            "ERROR: Desired validation layer not found.\n"
            "       Tried verifying support for '" + string(layer_name) + "'.\n"
        );
    }
}

void initialize_available_validation_layers ()
{
    vkEnumerateInstanceLayerProperties( get_available_layer_count(), nullptr );

    vector<VkLayerProperties> available_layers = *get_available_validation_layers();

    available_layers.resize( *get_available_layer_count() );
    vkEnumerateInstanceLayerProperties( get_available_layer_count(), available_layers.data() );
    
    set_available_validation_layers(available_layers);
}
