// DEFINE
#define OBJECT_INSTANCE uint32_t*


// INCLUDE
#include "../compact/compact.hpp"
#include "../bih/bih.hpp"


// DATA
#ifndef DEFINED_I
#define DEFINED_I
struct instance
{
    alignas(8) glm::vec2 position;
    alignas(4) float scale;
    alignas(4) float rotation;
    alignas(4) uint32_t data_one;
    alignas(4) uint32_t data_two;
};
#endif


// CODE
#ifndef DEFINED_IFUN
#define DEFINED_IFUN

OBJECT_INSTANCE create_instance (void* target_object, glm::vec2 position, float scale, float rotation);
indexed_aabb determine_instance_aabb (OBJECT_INSTANCE target_instance);
uint32_t encode_instance_data_one (uint16_t object_index, uint32_t instance_depth);
uint32_t encode_instance_data_two (uint32_t instance_depth, uint32_t object_bih_start);
uint16_t get_instance_object_index (OBJECT_INSTANCE target_instance);
uint32_t get_instance_depth (OBJECT_INSTANCE target_instance);
void set_instance_depth (OBJECT_INSTANCE target_instance, uint32_t new_depth);
uint32_t get_instance_bih_start (OBJECT_INSTANCE target_instance);
void update_instance_object (OBJECT_INSTANCE target_instance, void* new_object);
void update_instance_position (OBJECT_INSTANCE target_instance, glm::vec2 new_position);
void update_instance_scale (OBJECT_INSTANCE target_instance, float new_scale);
void update_instance_rotation (OBJECT_INSTANCE target_instance, float new_rotation);
void update_instance_depth (OBJECT_INSTANCE target_instance, uint32_t new_depth);
void delete_instances (vector<OBJECT_INSTANCE> target_instance);
void delete_object_instances (void* target_object);

#endif


// ACCESS
void set_instances_changed (bool change);
bool get_instances_changed ();
void set_instances (vector<instance> instances);
vector<instance>* get_instances ();
void set_instance_pointers (vector<OBJECT_INSTANCE> instance_pointers);
vector<uint32_t*>* get_instance_pointers ();
uint64_t get_maximum_instance_count ();
uint64_t get_maximum_instance_count_bytes ();
