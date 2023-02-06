// INCLUDE
#include "./instance.hpp"
#include "../object/object.hpp"


// CODE
OBJECT_INSTANCE create_instance (void* target_object, glm::vec2 position, float scale, float rotation)
{
    OBJECT target = reinterpret_cast<OBJECT>(target_object);

    OBJECT_INSTANCE new_instance_pointer = new uint32_t( get_instances()->size() );

    instance new_instance;
    new_instance.position = position;
    new_instance.scale = scale;
    new_instance.rotation = glm::radians( rotation );
    new_instance.data_one = encode_instance_data_one( target->index, get_instances()->size() );
    new_instance.data_two = encode_instance_data_two( get_instances()->size(), target->bih_range.x );

    get_instances()->push_back( new_instance );
    get_instance_pointers()->push_back( new_instance_pointer );

    store_instance_aabb( determine_instance_aabb(new_instance_pointer) );

    set_instances_changed(true);
    set_tlbih_changed(true);

    return new_instance_pointer;
}

indexed_aabb determine_instance_aabb (OBJECT_INSTANCE target_instance)
{
    OBJECT target_object = get_object_pointers()->at( get_instance_object_index( target_instance ));
    instance target = get_instances()->at( *target_instance );

    glm::vec2 object_dimensions = target_object->aabb.upper_bound - target_object->aabb.lower_bound;
    glm::vec2 aabb_offset = (target_object->aabb.lower_bound + object_dimensions / 2.0f) * target.scale;

    glm::vec2 top_corner = object_dimensions / 2.0f;
    glm::vec2 bottom_corner = top_corner * glm::vec2(1.0, -1.0);

    top_corner *= target.scale;
    bottom_corner *= target.scale;

    top_corner = rotate_point( top_corner, target.rotation );
    bottom_corner = rotate_point( bottom_corner, target.rotation );

    glm::vec2 dimensions_aabb_extent = {glm::max(abs(top_corner.x), abs(bottom_corner.x)), glm::max(abs(top_corner.y), abs(bottom_corner.y))};

    indexed_aabb output;
    output.aabb.lower_bound = -1.0f * dimensions_aabb_extent + target.position + aabb_offset;
    output.aabb.upper_bound = dimensions_aabb_extent + target.position + aabb_offset;
    output.index = *target_instance;

    return output;
}

uint32_t encode_instance_data_one (uint16_t object_index, uint32_t instance_depth)
{
    return ((uint32_t) object_index << 16) + ((instance_depth & 0x00FFFF00) >> 16);
}

uint32_t encode_instance_data_two (uint32_t instance_depth, uint32_t object_bih_start)
{
    return ((instance_depth & 0x000000FF) << 24) + object_bih_start;
}

uint16_t get_instance_object_index (OBJECT_INSTANCE target_instance)
{
    return (uint16_t) ((get_instances()->at( *target_instance ).data_one & 0xFFFF0000) >> 16);
}

uint32_t get_instance_depth (OBJECT_INSTANCE target_instance)
{
    return (uint32_t) (((get_instances()->at( *target_instance ).data_one & 0x0000FFFF) << 8) + ((get_instances()->at( *target_instance).data_two & 0xFF000000) >> 24));
}

void set_instance_depth (OBJECT_INSTANCE target_instance, uint32_t new_depth)
{
    get_instances()->at( *target_instance ).data_one = encode_instance_data_one( get_instance_object_index( target_instance ), new_depth );
    get_instances()->at( *target_instance ).data_two = encode_instance_data_two( new_depth, get_instance_bih_start( target_instance ) );
    set_instances_changed(true);
}

uint32_t get_instance_bih_start (OBJECT_INSTANCE target_instance)
{
    return get_instances()->at( *target_instance ).data_two & 0x00FFFFFF;
}

void update_instance_object (OBJECT_INSTANCE target_instance, void* new_object)
{
    OBJECT target = reinterpret_cast<OBJECT>(new_object);
    get_instances()->at( *target_instance ).data_one = encode_instance_data_one( target->index, get_instance_depth( target_instance ) );
    get_instances()->at( *target_instance ).data_two = encode_instance_data_two( get_instance_depth( target_instance ), target->bih_range.x );
    update_instance_aabb( determine_instance_aabb(target_instance), *target_instance );
    set_instances_changed(true);
    set_tlbih_changed(true);
}

void update_instance_position (OBJECT_INSTANCE target_instance, glm::vec2 new_position)
{
    get_instances()->at(*target_instance).position = new_position;
    update_instance_aabb( determine_instance_aabb(target_instance), *target_instance );
    set_instances_changed(true);
    set_tlbih_changed(true);
}

void update_instance_scale (OBJECT_INSTANCE target_instance, float new_scale)
{
    get_instances()->at(*target_instance).scale = new_scale;
    update_instance_aabb( determine_instance_aabb(target_instance), *target_instance );
    set_instances_changed(true);
    set_tlbih_changed(true);
}

void update_instance_rotation (OBJECT_INSTANCE target_instance, float new_rotation)
{
    get_instances()->at(*target_instance).rotation = glm::radians( new_rotation );
    update_instance_aabb( determine_instance_aabb(target_instance), *target_instance );
    set_instances_changed(true);
    set_tlbih_changed(true);
}

void update_instance_depth (OBJECT_INSTANCE target_instance, uint32_t new_depth)
{
    throw_error(new_depth >= get_instances()->size(),
        "\n"
        "ERROR: The provided new depth value for instance " + to_string(*target_instance) + "is outside of the possible depth range.\n"
        "       The depth value x of an instance has to satisfy 0 <= x < #instances."
    );

    uint32_t old_depth = get_instance_depth( target_instance );
    int sign = glm::sign( int(old_depth) - int(new_depth) );
    uint32_t lower = new_depth > old_depth ? old_depth : new_depth;
    uint32_t higher = new_depth > old_depth ? new_depth : old_depth;

    uint32_t depth;
    for (uint32_t i = 0; i < get_instances()->size(); i++)
    {
        depth = get_instance_depth( &i );
        depth += lower <= depth && depth <= higher ? sign * 1 : 0;

        set_instance_depth( &i, depth );
    }

    set_instance_depth( target_instance, new_depth );
}

void delete_instances (vector<OBJECT_INSTANCE> target_instances)
{
    sort(target_instances.begin(), target_instances.end(), [](OBJECT_INSTANCE instance_a, OBJECT_INSTANCE instance_b) {
        return (*instance_a < *instance_b);
    });

    int offset = 0;
    uint32_t current_target = *target_instances.at(0);
    for (int i = 0; i < get_instance_pointers()->size(); i++)
    {
        if (i > current_target && offset < target_instances.size())
        {
            offset++;
            if (offset < target_instances.size()) current_target = *target_instances.at(offset);
        }

        *get_instance_pointers()->at(i) -= offset;
        get_instance_aabbs()->at(i).index -= offset;
    }

    vector<uint32_t> removed_depths;

    OBJECT_INSTANCE target_instance;
    for (int i = 0; i < target_instances.size(); i++)
    {
        target_instance = target_instances[i];

        removed_depths.push_back( get_instance_depth( target_instance ) );

        get_instances()->erase( get_instances()->begin() + *target_instance );
        get_instance_pointers()->erase( get_instance_pointers()->begin() + *target_instance );
        get_instance_aabbs()->erase( get_instance_aabbs()->begin() + *target_instance );
        delete target_instance;
    }

    sort(removed_depths.begin(), removed_depths.end());

    OBJECT_INSTANCE current_pointer;
    uint16_t current_object_index;
    uint32_t current_depth;
    uint32_t new_depth;
    uint32_t current_bih_start;
    for (int i = 0; i < get_instance_pointers()->size(); i++)
    {
        current_pointer = get_instance_pointers()->at(i);
        current_object_index = get_instance_object_index( current_pointer );
        current_depth = get_instance_depth( current_pointer );
        new_depth = current_depth;
        current_bih_start = get_instance_bih_start( current_pointer );

        for (int j = 0; j < removed_depths.size(); j++)
        {
            if (current_depth > removed_depths[j])
            {
                new_depth -= 1;

                get_instances()->at( *current_pointer ).data_one = encode_instance_data_one(current_object_index, new_depth);
                get_instances()->at( *current_pointer).data_two = encode_instance_data_two(new_depth, current_bih_start);
            }
            else continue;
        }
    }

    set_instances_changed(true);
    set_tlbih_changed(true);
}

void delete_object_instances (void* target_object)
{
    OBJECT target = reinterpret_cast<OBJECT>(target_object);

    bool clean = false;
    int index = get_instances()->size() - 1;
    OBJECT_INSTANCE current_instance_pointer;

    if (index < 0) return;

    while (!clean)
    {
        current_instance_pointer = get_instance_pointers()->at(index);

        if ( get_instance_object_index( current_instance_pointer ) == target->index )
        {
            get_instances()->erase( get_instances()->begin() + *current_instance_pointer );
            get_instance_pointers()->erase( get_instance_pointers()->begin() + *current_instance_pointer );
            get_instance_aabbs()->erase( get_instance_aabbs()->begin() + *current_instance_pointer );
            delete current_instance_pointer;
        }

        index--;

        if (index < 0)
        {
            clean = true;
        }
    }

    uint16_t instance_object_index;
    uint32_t instance_depth;
    for (int i = 0; i < get_instance_pointers()->size(); i++)
    {
        current_instance_pointer = get_instance_pointers()->at(i);
        *current_instance_pointer = i;
        
        instance_object_index = get_instance_object_index( current_instance_pointer );
        instance_depth = get_instance_depth( current_instance_pointer );

        instance_object_index -= instance_object_index > target->index ? 1 : 0;

        get_instances()->at( *current_instance_pointer ).data_one = encode_instance_data_one( instance_object_index, instance_depth );
    }

    set_instances_changed(true);
    set_tlbih_changed(true);
}
