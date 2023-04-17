// INCLUDE
#include "./primitive.hpp"
#include "../object/object.hpp"


// OPERATION
uint32_t encode_operation (operation operation_type, uint32_t x_parameter)
{
    return operation_type + x_parameter;
}

uint32_t decode_operation (uint32_t operation, bool* using_intersection, bool* using_difference)
{
    *using_intersection = operation & 0b10000000000000000000000000000000;
    *using_difference = (operation & 0b01000000000000000000000000000000) && *using_intersection;
    if (*using_difference) *using_intersection = false;
    return operation - (*using_intersection ? 0b10000000000000000000000000000000 : 0) - (*using_difference ? 0b11000000000000000000000000000000 : 0);
}

uint32_t encode_depth (uint16_t depth, uint16_t y_parameter)
{
    uint32_t output = y_parameter << 16 | depth;
    return output;
}

uint32_t decode_depth (uint32_t encoded_depth, uint32_t* depth)
{
    *depth = (encoded_depth & 0x0000FFFF) >> 0;
    return (encoded_depth & 0xFFFF0000) >> 16;
}

glm::vec2 rotate_point (glm::vec2 point, float angle)
{
    return glm::mat2x2( glm::cos(angle), -1 * glm::sin(angle), glm::sin(angle), glm::cos(angle) ) * point;
}

glm::vec2 repetition_operation (glm::vec2 point, glm::vec2 spacing, float x_parameter, float y_parameter)
{
    glm::vec2 lower_limit = glm::vec2(-1.0 * x_parameter, -1.0 * y_parameter);
    glm::vec2 upper_limit = glm::vec2(x_parameter, y_parameter);
    return point - spacing * glm::clamp( glm::round(point / spacing), lower_limit, upper_limit );
}


// CONSTRUCTIVE SOLID GEOMETRY
// OPTIMISE: csg hierarchies can be non-flat if union operation available
indexed_aabb determine_csg_hierarchy_aabb (vector<int> primitive_indices, int primitive_offset, int hierarchy_depth)
{
    indexed_aabb csg_aabb;
    vector<bool> differences;
    vector<uint32_t> targets;

    primitive current_primitive = get_primitives()->at( primitive_indices[0] );
    bool using_intersection;
    bool using_difference;
    uint32_t x_parameter = decode_operation( current_primitive.x_parameter, &using_intersection, &using_difference );

    throw_error(using_intersection || using_difference, 
        "\n"
        "ERROR: The first primitive in a CSG hierarchy has to have a repetition operation.\n"
        "       This can be an implicit union (x and y = 0) or a domain operation (x and / or y > 0)."
    );

    csg_aabb.index = primitive_indices[0];
    csg_aabb.aabb = determine_primitive_aabb( primitive_indices[0] );

    differences.push_back( false );
    targets.push_back( 0 + primitive_offset );

    get_primitives()->at( primitive_indices[0] ).y_parameter += hierarchy_depth;

    for (int index = 1; index < primitive_indices.size(); index++)
    {
        current_primitive = get_primitives()->at( primitive_indices[index] );
        x_parameter = decode_operation(current_primitive.x_parameter, &using_intersection, &using_difference);

        throw_error(!using_intersection && !using_difference, 
            "\n"
            "ERROR: Any other primitive in a CSG hierarchy besides the first has to have an intersection or difference operation.\n"
        );

        targets.push_back( x_parameter + primitive_offset );

        throw_error(targets[index] >= index + primitive_offset, 
            "\n"
            "ERROR: Targets of an operation have to occur before the primitive performing the operation.\n"
        );

        get_primitives()->at( primitive_indices[index] ).x_parameter += primitive_offset;
        get_primitives()->at( primitive_indices[index] ).y_parameter += hierarchy_depth;
        get_primitives()->at( primitive_indices[index] ).colour = get_primitives()->at( primitive_indices[0] ).colour;
    }

    return csg_aabb;
}

int get_csg_hierarchies_size (vector< vector<int> > primitive_csg_hierarchies)
{
    int csg_hierarchies_size = 0;
    for (int i = 0; i < primitive_csg_hierarchies.size(); i++) csg_hierarchies_size += primitive_csg_hierarchies[i].size();
    return csg_hierarchies_size;
}


// PRIMITIVE
int create_primitive (primitive_constructor primitive_data, operation_constructor operation_data, colour colour_data)
{
    primitive new_primitive;

    primitive_constructor primitive_constructor_data;
    primitive_constructor_data.position = primitive_data.position;
    primitive_constructor_data.dimensions = primitive_data.dimensions;
    primitive_constructor_data.radius = primitive_data.radius;
    primitive_constructor_data.thickness = primitive_data.thickness;

    new_primitive.position = primitive_constructor_data.position;
    new_primitive.dimensions = primitive_constructor_data.dimensions;
    new_primitive.radius = primitive_constructor_data.radius;
    new_primitive.thickness = primitive_constructor_data.thickness;

    operation_constructor operation_constructor_data;
    operation_constructor_data.type = operation_data.type;
    operation_constructor_data.target = operation_data.target;
    operation_constructor_data.x_repeat = operation_data.x_repeat;
    operation_constructor_data.y_repeat = operation_data.y_repeat;
    operation_constructor_data.rotation = glm::radians( operation_data.rotation );

    throw_error( operation_constructor_data.type == 0 && operation_constructor_data.target > 0,
        "\n"
        "ERROR: Primitive operations of type REPETITION require a zeroed target."
    );

    new_primitive.x_parameter = encode_operation( operation_constructor_data.type, operation_constructor_data.target ? operation_constructor_data.target : operation_constructor_data.x_repeat );
    new_primitive.y_parameter = encode_depth( 0, operation_constructor_data.y_repeat );
    new_primitive.rotation = operation_constructor_data.rotation;

    colour colour_constructor_data;
    colour_constructor_data.red = colour_data.red;
    colour_constructor_data.green = colour_data.green;
    colour_constructor_data.blue = colour_data.blue;
    colour_constructor_data.alpha = colour_data.alpha;

    new_primitive.colour = encode_colour( colour_constructor_data );

    set_primitives_changed( true );

    return store_primitive( new_primitive );
}

float get_primitive_signed_distance (glm::vec2 point, int primitive_index)
{
    // get primitive
    primitive target = get_primitives()->at( primitive_index );

    // transform point
    point = point - target.position;

    // decode operation
    bool using_intersection;
    bool using_difference;
    uint32_t x_parameter = decode_operation( target.x_parameter, &using_intersection, &using_difference );

    // apply repetition operation if needed
    if (!using_intersection && !using_difference)
    {
        glm::vec2 spacing = (target.dimensions + target.radius) * 2.0f;
        point = repetition_operation( point, spacing, x_parameter, target.y_parameter );
    }

    // determine booleans
    bool is_box = glm::length( target.dimensions ) > 0.0;
    bool is_hollow = target.thickness > 0.0;

    // calculate dimensions of box
    // remains equal to point if sphere
    glm::vec2 dimensions = is_box ? glm::abs( point ) - target.dimensions : point;
    
    // calculate mirroring
    float clamping = is_box ? glm::min( glm::max( dimensions.x, dimensions.y ), 0.0f ) : 0.0;
    
    // determine point
    point = is_box ? glm::max( dimensions, glm::vec2(0.0) ) : point;
    
    // determine distance
    float signed_distance = glm::length( point ) + clamping - target.radius;
    
    // make hollow if specified
    signed_distance = is_hollow ? glm::abs( signed_distance ) - target.thickness : signed_distance;
    
    // return distance
    return signed_distance;
}

AABB determine_primitive_aabb (int primitive_index)
{
    primitive target = get_primitives()->at(primitive_index);

    glm::vec2 top_corner = {target.dimensions.x, target.dimensions.y};
    glm::vec2 bottom_corner = top_corner * glm::vec2(1.0, -1.0);

    top_corner = rotate_point( top_corner, target.rotation );
    bottom_corner = rotate_point( bottom_corner, target.rotation );

    glm::vec2 dimensions_aabb_extent = {glm::max(abs(top_corner.x), abs(bottom_corner.x)), glm::max(abs(top_corner.y), abs(bottom_corner.y))};

    AABB output;
    output.lower_bound = -1.0f * dimensions_aabb_extent - target.radius - target.thickness;
    output.upper_bound = dimensions_aabb_extent + target.radius + target.thickness;

    bool using_intersection;
    bool using_difference;
    uint32_t x_parameter = decode_operation( target.x_parameter, &using_intersection, &using_difference );
    uint32_t depth;
    uint32_t y_parameter = decode_depth( target.y_parameter, &depth );

    if (!using_intersection && !using_difference)
    {
        glm::vec2 dimensions = glm::abs(output.lower_bound) + output.upper_bound;
        output.upper_bound += dimensions * glm::vec2{x_parameter, y_parameter};
        output.lower_bound -= dimensions * glm::vec2{x_parameter, y_parameter};
    }

    output.lower_bound += target.position;
    output.upper_bound += target.position;

    return output;
}

void update_primitive_colour (void* target_object, int primitive_index, colour colour_data)
{
    OBJECT target = reinterpret_cast<OBJECT>(target_object);

    colour colour_constructor_data;
    colour_constructor_data.red = colour_data.red;
    colour_constructor_data.green = colour_data.green;
    colour_constructor_data.blue = colour_data.blue;
    colour_constructor_data.alpha = colour_data.alpha;

    get_primitives()->at(target->primitive_range.x + primitive_index).colour = encode_colour( colour_constructor_data );

    set_primitives_changed(true);
}


// UTILITY
glm::uvec2 determine_primitive_range (vector< vector<int> > primitive_csg_hierarchies)
{
    glm::uvec2 primitive_range;
    primitive_range.x = primitive_csg_hierarchies[0][0];
    primitive_range.y = 0;
    for (int i = 0; i < primitive_csg_hierarchies.size(); i++) primitive_range.y += primitive_csg_hierarchies[i].size();
    return primitive_range;
}

void determine_object_aabb (void* target_object)
{
    OBJECT target = reinterpret_cast<OBJECT>(target_object);

    AABB aabb;
    indexed_aabb current_aabb;
    for (int i = 0; i < target->primitive_aabbs.size(); i++)
    {
        current_aabb = target->primitive_aabbs.at(i);

        aabb.lower_bound.x = min(current_aabb.aabb.lower_bound.x, aabb.lower_bound.x);
        aabb.lower_bound.y = min(current_aabb.aabb.lower_bound.y, aabb.lower_bound.y);
        aabb.upper_bound.x = max(current_aabb.aabb.upper_bound.x, aabb.upper_bound.x);
        aabb.upper_bound.y = max(current_aabb.aabb.upper_bound.y, aabb.upper_bound.y);
    }

    target->aabb = aabb;
}

void delete_object_primitives (void* target_object)
{
    OBJECT target = reinterpret_cast<OBJECT>(target_object);

    primitive current_primitive;
    bool using_difference;
    bool using_intersection;
    for (int index = target->primitive_range.x + target->primitive_range.y; index < get_primitives()->size(); index++)
    {
        current_primitive = get_primitives()->at(index);
        decode_operation( current_primitive.x_parameter, &using_intersection, &using_difference );
        if (using_intersection || using_difference)
        {
            current_primitive.x_parameter -= target->primitive_range.y;
            get_primitives()->at(index) = current_primitive;
        }
    }

    get_primitives()->erase(
        get_primitives()->begin() + target->primitive_range.x,
        get_primitives()->begin() + target->primitive_range.x + target->primitive_range.y
    );

    OBJECT current_object;
    for (int index = target->index; index < get_object_pointers()->size(); index++)
    {
        current_object = get_object_pointers()->at(index);
        current_object->primitive_range.x -= target->primitive_range.y;

        for (int j = 0; j < current_object->primitive_aabbs.size(); j++)
        {
            current_object->primitive_aabbs[j].index -= target->primitive_range.y;
        }
    }

    set_primitives_changed(true);
}
