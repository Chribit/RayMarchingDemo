// DEFINE
#define OBJECT object*


// INCLUDE
#include "../compact/compact.hpp"
#include "../bih/bih.hpp"
#include "../primitive/primitive.hpp"
#include "../instance/instance.hpp"


// DATA
#ifndef DEFINED_O
#define DEFINED_O
struct object
{
    int index;
    AABB aabb;
    glm::uvec2 primitive_range;
    vector<indexed_aabb> primitive_aabbs;
    glm::uvec2 bih_range;
    vector<bounding_interval> bih;
};
#endif


// CODE
#ifndef DEFINED_OF
#define DEFINED_OF

OBJECT create_object (vector< vector<int> > primitive_csg_hierarchies);
void delete_object (OBJECT target_object, bool cleanup = false);
void cleanup_objects ();

#endif


// ACCESS
void store_object_pointer (OBJECT new_object);
void set_object_pointers (vector<OBJECT> object_pointers);
vector<OBJECT>* get_object_pointers ();
