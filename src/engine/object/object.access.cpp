// INCLUDE
#include "./object.hpp"


// DATA
static struct {

    // POINTERS
    vector<OBJECT> object_pointers;

} object_data;


// CODE
void store_object_pointer (OBJECT new_object)
{
    new_object->index = object_data.object_pointers.size();
    object_data.object_pointers.push_back(new_object);
}

void set_object_pointers (vector<OBJECT> object_pointers)
{
    object_data.object_pointers = object_pointers;
}

vector<OBJECT>* get_object_pointers ()
{
    return &object_data.object_pointers;
}
