// INCLUDE
#include "../compact/compact.hpp"


// DATA
#ifndef DEFINED_BI
#define DEFINED_BI
struct bounding_interval
{
    alignas(4) uint32_t link;
    alignas(4) uint32_t target;
    alignas(4) float coordinate;
};
#endif

#ifndef DEFINED_IAABB
#define DEFINED_IAABB
struct indexed_aabb
{
    uint32_t index;
    AABB aabb;
};
#endif


// UTILITY
glm::vec2 get_aabb_dimensions (AABB aabb);
glm::vec2 get_aabb_center (AABB aabb);
vector<glm::vec2> get_aabb_centers (vector<indexed_aabb> aabbs);
glm::vec2 sum_of_aabb_centers (vector<glm::vec2> aabb_centers);
glm::vec2 sum_of_aabb_distances_to_mean (vector<glm::vec2> aabb_centers, glm::vec2 mean);
uint32_t encode_bounding_interval_link (bool is_leaf, bool using_x, bool is_left, bool miss_terminates, uint32_t miss_link);
void determine_scene_aabb ();
void delete_object_blbih (void* target_object);


// CODE
void construct_blbih (void* target_object);
void construct_tlbih ();
vector<bounding_interval> construct_bounding_interval_hierarchy (vector<indexed_aabb> aabbs);
void construct_bounding_interval (vector<bounding_interval>* bih, vector<indexed_aabb> aabbs, int bih_size);


// ACCESS
void store_instance_aabb (indexed_aabb aabb);
void update_instance_aabb (indexed_aabb new_aabb, int index);
void set_instance_aabbs (vector<indexed_aabb> instance_aabbs);
vector<indexed_aabb>* get_instance_aabbs ();
void set_tlbih_changed (bool change);
bool get_tlbih_changed ();
void set_tlbih (vector<bounding_interval> tlbih);
vector<bounding_interval>* get_tlbih ();
uint64_t get_maximum_tlbi_count ();
uint64_t get_maximum_tlbi_count_bytes ();
void set_blbihs_changed (bool change);
bool get_blbihs_changed ();
vector<bounding_interval>* get_blbihs ();
uint64_t get_maximum_blbi_count ();
uint64_t get_maximum_blbi_count_bytes ();
