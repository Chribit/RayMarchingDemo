// INCLUDE
#include "./interface.hpp"


// CODE
void create_interface (string application_name, uvec2 window_size, function<void()> interface_code)
{
    register_component(
        any_cast<OBJECT_INSTANCE>(get_root().state("instance")),
        get_root_pointer()
    );

    initialise_events();

    on_initialisation(interface_code);

    start_engine("Signed Distance Field Engine", application_name, window_size.x, window_size.y);

    cleanup_interface();
}

void cleanup_interface ()
{
    for (pair shape : *get_shapes()) delete shape.second;
    get_shapes()->clear();
    for (pair component : *get_components()) delete component.second;
    get_components()->clear();
    for (pair line : *get_lines()) delete line.second;
    get_lines()->clear();
}
