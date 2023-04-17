// INCLUDE
#include "./traversal.hpp"
#include "../engine.hpp"


// CAMERA
glm::vec2 transform_point_for_camera (glm::vec2 point)
{
    return (point - get_push_constant_camera_position() * get_push_constant_camera_zoom()) / get_push_constant_camera_zoom();
}


// UTILITY
bool point_is_within_scene (glm::vec2 point)
{
    AABB scene_aabb = get_push_constant_scene_aabb();
    return point.x > scene_aabb.lower_bound.x && point.x < scene_aabb.upper_bound.x && point.y > scene_aabb.lower_bound.y && point.y < scene_aabb.upper_bound.y;
}


// TLBIH
bool point_is_left_in_tlbi (glm::vec2 point, uint32_t index)
{
    return point[ (get_tlbih()->at(index).link & 0x40000000) >> 30 ] < get_tlbih()->at(index).coordinate;
}

bool point_is_center_in_tlbi (glm::vec2 point, uint32_t index)
{
    return point[ (get_tlbih()->at(index).link & 0x40000000) >> 30 ] == get_tlbih()->at(index).coordinate;
}

bool point_intersects_tlbi (glm::vec2 point, uint32_t index)
{
    return ((point_is_left_in_tlbi(point, index) || point_is_center_in_tlbi(point, index)) && !((get_tlbih()->at(index).link & 0x20000000) >> 29)) || ((!point_is_left_in_tlbi(point, index) || point_is_center_in_tlbi(point, index)) && ((get_tlbih()->at(index).link & 0x20000000) >> 29));
}

bool current_tlbi_terminates (uint32_t index)
{
    return (bool) (get_tlbih()->at(index).link & 0x10000000);
}

uint32_t current_tlbi_miss_link (uint32_t index)
{
    return get_tlbih()->at(index).link & 0x0FFFFFFF;
}

uint32_t current_tlbi_target (uint32_t index)
{
    return get_tlbih()->at(index).target;
}

bool current_tlbi_is_leaf (uint32_t index)
{
    return (bool) (get_tlbih()->at(index).link & 0x80000000);  
}


// INSTANCE
uint32_t get_instance_object_index (uint32_t index)
{
    return (get_instances()->at(index).data_one & 0xFFFF0000) >> 16;
}

uint32_t get_instance_depth (uint32_t index)
{
    return ((get_instances()->at(index).data_one & 0x0000FFFF) << 8) + ((get_instances()->at(index).data_two & 0xFF000000) >> 24);
}

uint32_t get_instance_blbi_start (uint32_t index)
{
    return get_instances()->at(index).data_two & 0x00FFFFFF;
}

glm::vec2 get_instance_position (uint32_t index)
{
    return get_instances()->at(index).position;
}

float get_instance_rotation (uint32_t index)
{
    return get_instances()->at(index).rotation;
}

float get_instance_scale (uint32_t index)
{
    return get_instances()->at(index).scale;
}

glm::vec2 translate_point_for_instance (uint32_t index, glm::vec2 point)
{
    return point - get_instance_position(index);
}

glm::vec2 rotate_point_for_instance (uint32_t index, glm::vec2 point)
{
    return glm::mat2x2( glm::cos( get_instance_rotation(index) ), -1 * glm::sin( get_instance_rotation(index) ), glm::sin( get_instance_rotation(index) ), glm::cos( get_instance_rotation(index) ) ) * point;
}

glm::vec2 scale_point_for_instance (uint32_t index, glm::vec2 point)
{
    return point / get_instance_scale(index);
}


// BLBIH
bool point_is_left_in_blbi (glm::vec2 point, uint32_t index)
{
    return point[ (get_blbihs()->at(index).link & 0x40000000) >> 30 ] < get_blbihs()->at(index).coordinate;
}

bool point_is_center_in_blbi (glm::vec2 point, uint32_t index)
{
    return point[ (get_blbihs()->at(index).link & 0x40000000) >> 30 ] == get_blbihs()->at(index).coordinate;
}

bool point_intersects_blbi (glm::vec2 point, uint32_t index)
{
    return ((point_is_left_in_blbi(point, index) || point_is_center_in_blbi(point, index)) && !((get_blbihs()->at(index).link & 0x20000000) >> 29)) || ((!point_is_left_in_blbi(point, index) || point_is_center_in_blbi(point, index)) && ((get_blbihs()->at(index).link & 0x20000000) >> 29));
}

bool current_blbi_terminates (uint32_t index)
{
    return (bool) (get_blbihs()->at(index).link & 0x10000000);
}

uint32_t current_blbi_miss_link (uint32_t index)
{
    return get_blbihs()->at(index).link & 0x0FFFFFFF;
}

uint32_t current_blbi_target (uint32_t index)
{
    return get_blbihs()->at(index).target; 
}

bool current_blbi_is_leaf (uint32_t index)
{
    return (bool) (get_blbihs()->at(index).link & 0x80000000);  
}


// PRIMITIVE
bool primitive_uses_operation (uint32_t index)
{
    return (bool) (get_primitives()->at(index).x_parameter & 0b10000000000000000000000000000000);
}

bool primitive_uses_intersection (uint32_t index)
{
    return (bool) (get_primitives()->at(index).x_parameter & 0b10000000000000000000000000000000) && ! ((bool) (get_primitives()->at(index).x_parameter & 0b01000000000000000000000000000000));
}

bool primitive_uses_difference (uint32_t index)
{
    return (bool) (get_primitives()->at(index).x_parameter & 0b11000000000000000000000000000000); 
}

uint32_t get_primitive_x_parameter (uint32_t index)
{
    return get_primitives()->at(index).x_parameter & 0x3FFFFFFF;
}

uint32_t get_primitive_depth (uint32_t index)
{
    return get_primitives()->at(index).y_parameter & 0x0000FFFF;
}

uint32_t get_primitive_y_parameter (uint32_t index)
{
    return (get_primitives()->at(index).y_parameter & 0xFFFF0000) >> 16;
}

glm::vec2 get_primitive_position (uint32_t index)
{
    return get_primitives()->at(index).position;
}

glm::vec2 get_primitive_dimensions (uint32_t index)
{
    return get_primitives()->at(index).dimensions;
}

float get_primitive_radius (uint32_t index)
{
    return get_primitives()->at(index).radius; 
}

float get_primitive_thickness (uint32_t index)
{
    return get_primitives()->at(index).thickness;
}

float get_primitive_rotation (uint32_t index)
{
    return get_primitives()->at(index).rotation;
}

glm::vec3 get_primitive_colour (uint32_t index)
{    
    return glm::vec3(
        ((get_primitives()->at(index).colour & 0xFF000000) >> 24) / 255.0,
        ((get_primitives()->at(index).colour & 0x00FF0000) >> 16) / 255.0,
        ((get_primitives()->at(index).colour & 0x0000FF00) >> 8) / 255.0
    );
}

float get_primitive_opacity (uint32_t index)
{
    return (get_primitives()->at(index).colour & 0x000000FF) / 255.0;
}

glm::vec2 translate_point_for_primitive (uint32_t index, glm::vec2 point)
{
    return point - get_primitive_position(index);
}

glm::vec2 rotate_point_for_primitive (uint32_t index, glm::vec2 point)
{
    return glm::mat2x2( glm::cos( get_primitive_rotation(index) ), -1 * glm::sin( get_primitive_rotation(index) ), glm::sin( get_primitive_rotation(index) ), glm::cos( get_primitive_rotation(index) ) ) * point;  
}

glm::vec2 get_primitive_repetition_spacing (uint32_t index)
{
    return (get_primitive_dimensions(index) + get_primitive_radius(index)) * 2.0f;
}

glm::vec2 repeat_point_for_primitive (uint32_t index, glm::vec2 point)
{
    return point - get_primitive_repetition_spacing(index) * glm::clamp( glm::round(point / get_primitive_repetition_spacing(index)), glm::vec2(-1.0 * get_primitive_x_parameter(index), -1.0 * get_primitive_y_parameter(index)), glm::vec2(get_primitive_x_parameter(index), get_primitive_y_parameter(index)) );
}

bool primitive_has_dimensions (uint32_t index)
{
    return glm::length( get_primitive_dimensions(index) ) > 0.0;
}

bool primitive_is_hollow (uint32_t index)
{
    return get_primitive_thickness(index) > 0.0;
}

float get_primitive_signed_distance (uint32_t index, glm::vec2 point)
{
    point = translate_point_for_primitive(index, point);
    point = rotate_point_for_primitive(index, point);
    if (!primitive_uses_operation(index)) point = repeat_point_for_primitive(index, point);

    // calculate dimensions of box
    // remains equal to point if sphere
    glm::vec2 dimensions = primitive_has_dimensions(index) ? glm::abs( point ) - get_primitive_dimensions(index) : point;
    
    // calculate mirroring
    float clamping = primitive_has_dimensions(index) ? glm::min( glm::max( dimensions.x, dimensions.y ), 0.0f ) : 0.0;
    
    // determine point
    point = primitive_has_dimensions(index) ? glm::max( dimensions, glm::vec2(0.0) ) : point;
    
    // determine distance
    float signed_distance = glm::length( point ) + clamping - get_primitive_radius(index);
    
    // make hollow if specified
    signed_distance = primitive_is_hollow(index) ? glm::abs( signed_distance ) - get_primitive_thickness(index) : signed_distance;
    
    // return distance
    return signed_distance;
}

bool point_intersects_primitive (uint32_t index, glm::vec2 point, float scale)
{
    float distance = get_primitive_signed_distance(index, point);

    while (true)
    {
        index++;

        if ( index >= get_primitives()->size() ) break;
        if ( !primitive_uses_operation(index) ) break;

        if ( primitive_uses_intersection(index) )
        {
            distance = glm::max(get_primitive_signed_distance( index, point ), distance);
        }
        else
        {
            distance = glm::max(-1 * get_primitive_signed_distance( index, point ), distance);
        }
    }

    distance = distance * scale;

    return distance <= 0;
}

uint32_t determine_colour_depth (uint32_t instance_index, uint32_t primitive_index)
{
    return (get_instance_depth( instance_index ) << 10) + get_primitive_depth( primitive_index );
}


// CODE
colour determine_pixel_colour (glm::vec2 point)
{
    glm::vec3 colour = glm::vec3(0.0);
    uint32_t colour_depth = 0;
    glm::vec2 transformed_point = point;
    uint32_t current_tlbi = 0;
    uint32_t current_blbi = 0;

    if ( !point_is_within_scene( point )) return { (uint32_t) (colour.x * 255), (uint32_t) (colour.y * 255), (uint32_t) (colour.z * 255), 255 };

    while (true) {

        if ( point_intersects_tlbi( point, current_tlbi )) {
            if ( current_tlbi_is_leaf( current_tlbi )) {

                transformed_point = translate_point_for_instance( current_tlbi_target(current_tlbi), point );
                transformed_point = rotate_point_for_instance( current_tlbi_target(current_tlbi), transformed_point );
                transformed_point = scale_point_for_instance( current_tlbi_target(current_tlbi), transformed_point );

                current_blbi = get_instance_blbi_start( current_tlbi_target(current_tlbi) );

                while (true) {
                    
                    if ( point_intersects_blbi( transformed_point, current_blbi )) {
                        if ( current_blbi_is_leaf( current_blbi )) {
                            
                            if ( point_intersects_primitive( current_blbi_target(current_blbi), transformed_point, get_instance_scale( current_tlbi_target(current_tlbi))))
                            {
                                if (get_primitive_opacity( current_blbi_target(current_blbi) ) > 0.0)
                                {
                                    colour = colour_depth > determine_colour_depth(current_tlbi_target(current_tlbi), current_blbi_target(current_blbi)) ? colour : get_primitive_colour( current_blbi_target(current_blbi) );

                                    colour_depth = glm::max(
                                        determine_colour_depth(current_tlbi_target(current_tlbi), current_blbi_target(current_blbi)),
                                        colour_depth
                                    );
                                }
                            }

                            if ( current_blbi_terminates( current_blbi )) break;
                        }

                        current_blbi++;
                        continue;
                    }
                    else {
                        
                        if ( current_blbi_terminates( current_blbi )) break;

                        current_blbi = current_blbi_miss_link( current_blbi );
                        continue;
                    }
                }

                if ( current_tlbi_terminates( current_tlbi )) return { (uint32_t) (colour.x * 255), (uint32_t) (colour.y * 255), (uint32_t) (colour.z * 255), 255 };
            }

            current_tlbi++;
            continue;
        }
        else {
            if ( current_tlbi_terminates( current_tlbi )) return { (uint32_t) (colour.x * 255), (uint32_t) (colour.y * 255), (uint32_t) (colour.z * 255), 255 };

            current_tlbi = current_tlbi_miss_link( current_tlbi );
            continue;
        }
    }
}

void determine_pixel_object ()
{
    glm::vec2 point = get_mouse_coordinates();

    vector<OBJECT_INSTANCE> picked_objects;
    glm::vec2 transformed_point = point;
    uint32_t current_tlbi = 0;
    uint32_t current_blbi = 0;

    if ( !point_is_within_scene( point ))
    {
        set_pixel_objects(picked_objects);
        return;
    }

    while (true) {

        if ( point_intersects_tlbi( point, current_tlbi )) {

            if ( current_tlbi_is_leaf( current_tlbi )) {

                transformed_point = translate_point_for_instance( current_tlbi_target(current_tlbi), point );
                transformed_point = rotate_point_for_instance( current_tlbi_target(current_tlbi), transformed_point );
                transformed_point = scale_point_for_instance( current_tlbi_target(current_tlbi), transformed_point );

                current_blbi = get_instance_blbi_start( current_tlbi_target(current_tlbi) );

                while (true) {
                    
                    if ( point_intersects_blbi( transformed_point, current_blbi )) {
                        if ( current_blbi_is_leaf( current_blbi )) {
                            
                            if ( point_intersects_primitive( current_blbi_target(current_blbi), transformed_point, get_instance_scale( current_tlbi_target(current_tlbi))))
                            {
                                if (get_primitive_opacity( current_blbi_target(current_blbi) ) > 0.0)
                                {
                                    picked_objects.push_back(
                                        get_instance_pointers()->at( current_tlbi_target(current_tlbi) )
                                    );
                                }
                            }

                            if ( current_blbi_terminates( current_blbi )) break;
                        }

                        current_blbi++;
                        continue;
                    }
                    else {
                        
                        if ( current_blbi_terminates( current_blbi )) break;

                        current_blbi = current_blbi_miss_link( current_blbi );
                        continue;
                    }
                }

                if ( current_tlbi_terminates( current_tlbi ))
                {
                    set_pixel_objects(picked_objects);
                    return;
                }
            }

            current_tlbi++;
            continue;
        }
        else {
            if ( current_tlbi_terminates( current_tlbi ))
            {
                set_pixel_objects(picked_objects);
                return;
            }

            current_tlbi = current_tlbi_miss_link( current_tlbi );
            continue;
        }
    }
}
