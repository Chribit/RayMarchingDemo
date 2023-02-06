// INCLUDE
#include "../compact/compact.hpp"
#include "../colour/colour.hpp"
#include "../bih/bih.hpp"


// DATA
#ifndef DEFINED_P
#define DEFINED_P
struct primitive
{
    alignas(8) glm::vec2 position;
    alignas(8) glm::vec2 dimensions;
    alignas(4) float radius;
    alignas(4) float thickness;
    alignas(4) uint32_t x_parameter;
    alignas(4) uint32_t y_parameter;
    alignas(4) float rotation;
    alignas(4) uint32_t colour;
};
#endif

#ifndef DEFINED_PCON
#define DEFINED_PCON
struct primitive_constructor
{
    glm::vec2 position = {0.0, 0.0};
    glm::vec2 dimensions = {0.0, 0.0};
    float radius = 0.0;
    float thickness = 0.0;
};
#endif

#ifndef DEFINED_OP
#define DEFINED_OP
enum operation
{
    REPETITION =   0b00000000000000000000000000000000,
    INTERSECTION = 0b10000000000000000000000000000000,
    DIFFERENCE =   0b11000000000000000000000000000000
};
#endif

#ifndef DEFINED_OPCON
#define DEFINED_OPCON
struct operation_constructor
{
    operation type;
    uint32_t target = 0;
    uint32_t x_repeat = 0;
    uint16_t y_repeat = 0;
    float rotation = 0.0;
};
#endif


// OPERATION
uint32_t encode_operation (operation operation_type, uint32_t x_parameter);
uint32_t decode_operation (uint32_t operation, bool* using_intersection, bool* using_difference);
uint32_t encode_depth (uint16_t depth, uint16_t y_parameter);
uint32_t decode_depth (uint32_t encoded_depth, uint32_t* depth);
glm::vec2 rotate_point (glm::vec2 point, float angle);
glm::vec2 repetition_operation (glm::vec2 point, glm::vec2 spacing, float x_parameter, float y_parameter);


// CONSTRUCTIVE SOLID GEOMETRY
indexed_aabb determine_csg_hierarchy_aabb (vector<int> primitive_indices, int primitive_offset, int hierarchy_depth);
int get_csg_hierarchies_size (vector< vector<int> > primitive_csg_hierarchies);


// PRIMITIVE
int create_primitive (primitive_constructor primitive_data, operation_constructor operation_data, colour colour_data);
float get_primitive_signed_distance (glm::vec2 point, int primitive_index);
AABB determine_primitive_aabb (int primitive_index);
void update_primitive_colour (void* target_object, int primitive_index, colour colour_data);


// UTILITY
glm::uvec2 determine_primitive_range (vector< vector<int> > primitive_csg_hierarchies);
void determine_object_aabb (void* target_object);
void delete_object_primitives (void* target_object);


// ACCESS
void set_primitives_changed (bool change);
bool get_primitives_changed ();
int store_primitive (primitive new_primitive);
void set_primitives (vector<primitive> primitives);
vector<primitive>* get_primitives ();
uint64_t get_maximum_primitive_count ();
uint64_t get_maximum_primitive_count_bytes ();
