// INCLUDE
#include "./bih.hpp"
#include "../object/object.hpp"
#include "../memory/memory.hpp"


// UTILITY
glm::vec2 get_aabb_dimensions (AABB aabb)
{
    return aabb.upper_bound - aabb.lower_bound;
}

glm::vec2 get_aabb_center (AABB aabb)
{
    return aabb.lower_bound + get_aabb_dimensions(aabb) / 2.0f;
}

vector<glm::vec2> get_aabb_centers (vector<indexed_aabb> aabbs)
{
    vector<glm::vec2> output;
    for (int i = 0; i < aabbs.size(); i++)
    {
        output.push_back( get_aabb_center(aabbs[i].aabb) );
    }
    return output;
}

glm::vec2 sum_of_aabb_centers (vector<glm::vec2> aabb_centers)
{
    glm::vec2 output = {0.0, 0.0};
    for (int i = 0; i < aabb_centers.size(); i++)
    {
        output += aabb_centers[i];
    }
    return output;
}

glm::vec2 sum_of_aabb_distances_to_mean (vector<glm::vec2> aabb_centers, glm::vec2 mean)
{
    glm::vec2 output = {0.0, 0.0};
    for (int i = 0; i < aabb_centers.size(); i++)
    {
        output += glm::abs( mean - aabb_centers[i] );
    }
    return output;
}

uint32_t encode_bounding_interval_link (bool is_leaf, bool using_x, bool is_left, bool miss_terminates, uint32_t miss_link)
{
    return (is_leaf ? 0x80000000 : 0) + (using_x ? 0 : 0x40000000) + (is_left ? 0 : 0x20000000) + (miss_terminates ? 0x10000000 : 0) + miss_link;
}

void determine_scene_aabb ()
{
    AABB scene_aabb;
    scene_aabb.lower_bound = glm::vec2(1000000.0);
    scene_aabb.upper_bound = glm::vec2(-1000000.0);

    AABB current_instance_aabb;

    for (int i = 0; i < get_instance_aabbs()->size(); i++)
    {
        current_instance_aabb = get_instance_aabbs()->at(i).aabb;

        if (current_instance_aabb.lower_bound.x < scene_aabb.lower_bound.x) scene_aabb.lower_bound.x = current_instance_aabb.lower_bound.x;
        if (current_instance_aabb.lower_bound.y < scene_aabb.lower_bound.y) scene_aabb.lower_bound.y = current_instance_aabb.lower_bound.y;
        if (current_instance_aabb.upper_bound.x > scene_aabb.upper_bound.x) scene_aabb.upper_bound.x = current_instance_aabb.upper_bound.x;
        if (current_instance_aabb.upper_bound.y > scene_aabb.upper_bound.y) scene_aabb.upper_bound.y = current_instance_aabb.upper_bound.y;
    }

    if (get_instance_aabbs()->size() == 0)
    {
        scene_aabb.lower_bound = glm::vec2(0.0);
        scene_aabb.upper_bound = glm::vec2(0.0);
    }

    set_push_constant_scene_aabb(scene_aabb);
}

void delete_object_blbih (void* target_object)
{
    OBJECT target = reinterpret_cast<OBJECT>(target_object);

    get_blbihs()->erase(
        get_blbihs()->begin() + target->bih_range.x,
        get_blbihs()->begin() + target->bih_range.x + target->bih_range.y
    );

    OBJECT current_other_object;
    int offset;
    for (int i = target->index + 1; i < get_object_pointers()->size(); i++)
    {
        current_other_object = get_object_pointers()->at(i);
        current_other_object->bih_range.x -= target->bih_range.y;

        for (int j = 0; j < current_other_object->bih.size(); j++)
        {
            offset = (target->bih_range.y << 12) + target->primitive_range.y;

            get_blbihs()->at( current_other_object->bih_range.x + j ).link -= offset;
            current_other_object->bih.at(j).link -= offset;
        }
    }

    for (int i = 0; i < get_instance_aabbs()->size(); i++)
    {
        if (get_instance_aabbs()->at(i).index > target->bih_range.x)
        {
            get_instance_aabbs()->at(i).index -= target->bih_range.y;
        }
    }
}


// BOUNDING INTERVAL HIERARCHIES
void construct_blbih (void* target_object)
{
    OBJECT target = reinterpret_cast<OBJECT>(target_object);
    target->bih = construct_bounding_interval_hierarchy( target->primitive_aabbs );
    target->bih_range = glm::uvec2(get_blbihs()->size(), target->bih.size());

    for (int i = 0; i < target->bih.size(); i++)
    {
        target->bih.at(i).link += target->bih_range.x;
    }

    get_blbihs()->insert(
        get_blbihs()->end(),
        target->bih.begin(),
        target->bih.end()
    );

    set_blbihs_changed(true);
}

void construct_tlbih ()
{
    set_tlbih( construct_bounding_interval_hierarchy( *get_instance_aabbs() ) );
}

vector<bounding_interval> construct_bounding_interval_hierarchy (vector<indexed_aabb> aabbs)
{
    // initialise bounding interval vector
    vector<bounding_interval> bih;

    // return if no aabbs provided
    if (aabbs.size() == 0) return bih;

    // return single terminal left leaf interval if only one aabb provided
    if (aabbs.size() == 1) 
    {
        bounding_interval left_bi;
        left_bi.link = encode_bounding_interval_link(true, true, true, true, 0);
        left_bi.target = aabbs[0].index;
        left_bi.coordinate = aabbs[0].aabb.upper_bound[0];

        bih.push_back(left_bi);

        return bih;
    }

    // determine final size of bounding interval hierarchy in nodes
    // subtract 2 instead of 1, since the root node is imaginary
    int bih_size = 2 * aabbs.size() - 2;

    // recursively construct bounding intervals
    construct_bounding_interval( &bih, aabbs, bih_size );

    // remove miss link from final bounding interval
    // set termination bit instead
    bih[ bih.size() - 1 ].link += 0x10000000 - bih_size;

    // return bounding interval hierarchy
    return bih;
}

void construct_bounding_interval (vector<bounding_interval>* bih, vector<indexed_aabb> aabbs, int bih_size)
{
    // get aabb center coordinates
    vector<glm::vec2> aabb_centers = get_aabb_centers( aabbs );

    // get average center coordinates of given aabbs
    glm::vec2 position_mean = sum_of_aabb_centers( aabb_centers ) / (float) aabbs.size();

    // get average distance to position_mean
    glm::vec2 distance_mean =  sum_of_aabb_distances_to_mean( aabb_centers, position_mean ) / (float) aabbs.size();

    // determine dimension to use based on highest average distance to mean
    // simply the inversion of the condition to get the correct index
    int dimension = !( distance_mean.x >= distance_mean.y );

    // get aabb index vector to sort
    vector<int> aabb_indices( aabb_centers.size() );
    iota( aabb_indices.begin(), aabb_indices.end(), 0 );

    // sort aabb indices along chosen dimension in centers
    sort(aabb_indices.begin(), aabb_indices.end(), [aabb_centers, dimension] (int a, int b) {
        return aabb_centers[a][dimension] < aabb_centers[b][dimension];
    });

    // get half size of aabbs
    int split_index = (int) ceil(aabb_indices.size() / 2.0);

    // initialise bounding intervals
    bounding_interval left_bi;
    bounding_interval right_bi;

    // initialise bounding interval coordinates
    left_bi.coordinate = -1000000000.0;
    right_bi.coordinate = 1000000000.0;

    // initialise bins
    vector<indexed_aabb> left_bin;
    vector<indexed_aabb> right_bin;

    // initialise loop variable
    float current_coordinate;

    // fill left bin and determine left coordinate
    for (int i = 0; i < split_index; i++)
    {
        left_bin.push_back( aabbs[ aabb_indices[i] ] );

        current_coordinate = aabbs[ aabb_indices[i] ].aabb.upper_bound[ dimension ];
        left_bi.coordinate = current_coordinate > left_bi.coordinate ? current_coordinate : left_bi.coordinate;
    }

    // fill right bin and determine right coordinate 
    for (int i = split_index; i < aabb_indices.size(); i++)
    {
        right_bin.push_back( aabbs[ aabb_indices[i] ] );

        current_coordinate = aabbs[ aabb_indices[i] ].aabb.lower_bound[ dimension ];
        right_bi.coordinate = current_coordinate < right_bi.coordinate ? current_coordinate : right_bi.coordinate;
    }

    // determine if either bounding interval is a leaf node
    bool left_is_leaf = left_bin.size() == 1;
    bool right_is_leaf = right_bin.size() == 1;

    // store index of the left interval to be inserted for miss link update if node is internal
    int left_index = bih->size();

    // determine link bits of left bounding interval
    // miss link is set to next node if the left bounding interval is a leaf
    // target is set to index of first aabb element in left bin, needed for leaf nodes --> data there is irrelevant if internal
    uint32_t miss_link = left_is_leaf ? bih->size() + 1 : 0;
    left_bi.link = encode_bounding_interval_link( left_is_leaf, dimension == 0, true, false, miss_link );
    left_bi.target = left_bin[0].index;

    // store left bounding interval in bih vector
    bih->push_back(left_bi);

    // call bounding interval construction recursively if left bounding interval is not a leaf
    if (!left_is_leaf) 
    {
        // call bounding interval construction
        construct_bounding_interval( bih, left_bin, bih_size );

        // set miss link of left internal node bounding interval
        // miss link points to current size of bih
        bih->at(left_index).link += bih->size();
    }

    // store index of the right interval to be inserted for miss link update if node is internal
    int right_index = bih->size();

    // determine link bits of right bounding interval
    // miss link is set to next node if the right bounding interval is a leaf
    // target is set to index of first aabb element in right bin, needed for leaf nodes --> data there is irrelevant if internal
    miss_link = right_is_leaf ? bih->size() + 1 : 0;
    right_bi.link = encode_bounding_interval_link( right_is_leaf, dimension == 0, false, false, miss_link );
    right_bi.target = right_bin[0].index;

    // store right bounding interval in bih vector
    bih->push_back(right_bi);

    // call bounding interval construction recursively if right bounding interval is not a leaf
    if (!right_is_leaf)
    {

        // call bounding interval construction
        construct_bounding_interval( bih, right_bin, bih_size );

        // set miss link of right internal node bounding interval if the current bih size is not the maximum bih size
        // if size is maximum, the right interval is terminal and the representative bit is set
        bih->at(right_index).link += bih->size() == bih_size ? 0x10000000 : bih->size();
    }
}
