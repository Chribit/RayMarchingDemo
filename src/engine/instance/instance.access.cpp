// INCLUDE
#include "./instance.hpp"


// DATA
static struct {

    // FLAGS
    bool instances_changed = false;

    // ELEMENTS
    vector<instance> instances;
    vector<OBJECT_INSTANCE> instance_pointers;

    // MAXIMUM SIZES
    // maximum of 2048000 instances
    uint64_t maximum_instance_count = 8000 * 256;
    uint64_t maximum_instance_count_bytes = maximum_instance_count * sizeof(instance);

} instance_data;


// CODE
void set_instances_changed (bool change)
{
    instance_data.instances_changed = change;
}

bool get_instances_changed ()
{
    return instance_data.instances_changed;
}

void set_instances (vector<instance> instances)
{
    instance_data.instances = instances;
}

vector<instance>* get_instances ()
{
    return &instance_data.instances;
}

void set_instance_pointers (vector<OBJECT_INSTANCE> instance_pointers)
{
    instance_data.instance_pointers = instance_pointers;
}

vector<OBJECT_INSTANCE>* get_instance_pointers ()
{
    return &instance_data.instance_pointers;
}

uint64_t get_maximum_instance_count ()
{
    return instance_data.maximum_instance_count;
}

uint64_t get_maximum_instance_count_bytes ()
{
    return instance_data.maximum_instance_count_bytes;
}
