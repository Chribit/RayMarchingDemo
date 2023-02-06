// INCLUDE
#include "./primitive.hpp"


// DATA
static struct {

    // FLAGS
    bool primitives_changed = false;

    // ELEMENTS
    vector<primitive> primitives;

    // MAXIMUM SIZES
    // maximum of 512000 primitives
    uint64_t maximum_primitive_count = 2000 * 256;
    uint64_t maximum_primitive_count_bytes = maximum_primitive_count * sizeof(primitive);

} primitive_data;



// CODE
void set_primitives_changed (bool change)
{
    primitive_data.primitives_changed = change;
}

bool get_primitives_changed ()
{
    return primitive_data.primitives_changed;
}

int store_primitive (primitive new_primitive)
{
    primitive_data.primitives.push_back(new_primitive);
    return primitive_data.primitives.size() - 1;
}

void set_primitives (vector<primitive> primitives)
{
    primitive_data.primitives = primitives;
}

vector<primitive>* get_primitives ()
{
    return &primitive_data.primitives;
}

uint64_t get_maximum_primitive_count ()
{
    return primitive_data.maximum_primitive_count;
}

uint64_t get_maximum_primitive_count_bytes ()
{
    return primitive_data.maximum_primitive_count_bytes;
}
