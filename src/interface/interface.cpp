// INCLUDE
#include "./interface.hpp"


// CODE
void create_interface (string application_name, function<void()> interface_code)
{
    on_initialisation(interface_code);

    start_engine("Signed Distance Field Engine", application_name, 960, 540);

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
