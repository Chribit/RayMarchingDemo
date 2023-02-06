// INCLUDE
#include "./bih.hpp"


// DATA
static struct {

    // FLAGS
    bool blbihs_changed = false;
    bool tlbih_changed = false;

    // ELEMENTS
    vector<bounding_interval> tlbih;
    vector<bounding_interval> blbih;
    vector<indexed_aabb> instance_aabbs;

    // MAXIMUM SIZES
    // maximum of 4096000 tlbis
    uint64_t maximum_tlbi_count = 16000 * 256;
    uint64_t maximum_tlbi_count_bytes = maximum_tlbi_count * sizeof(bounding_interval);
    // maximum of 1024000 blbis
    uint64_t maximum_blbi_count = 4000 * 256;
    uint64_t maximum_blbi_count_bytes = maximum_blbi_count * sizeof(bounding_interval);

} bih_data;


// INSTANCES
void store_instance_aabb (indexed_aabb aabb)
{
    bih_data.instance_aabbs.push_back(aabb);
}

void update_instance_aabb (indexed_aabb new_aabb, int index)
{
    bih_data.instance_aabbs.at(index) = new_aabb;
}

void set_instance_aabbs (vector<indexed_aabb> instance_aabbs)
{
    bih_data.instance_aabbs = instance_aabbs;
}

vector<indexed_aabb>* get_instance_aabbs ()
{
    return &bih_data.instance_aabbs;
}


// TLBIH
void set_tlbih_changed (bool change)
{
    bih_data.tlbih_changed = change;
}

bool get_tlbih_changed ()
{
    return bih_data.tlbih_changed;
}

void set_tlbih (vector<bounding_interval> tlbih)
{
    bih_data.tlbih = tlbih;
}

vector<bounding_interval>* get_tlbih ()
{
    return &bih_data.tlbih;
}

uint64_t get_maximum_tlbi_count ()
{
    return bih_data.maximum_tlbi_count;
}

uint64_t get_maximum_tlbi_count_bytes ()
{
    return bih_data.maximum_tlbi_count_bytes;
}


// BLBIH
void set_blbihs_changed (bool change)
{
    bih_data.blbihs_changed = change;
}

bool get_blbihs_changed ()
{
    return bih_data.blbihs_changed;
}

vector<bounding_interval>* get_blbihs ()
{
    return &bih_data.blbih;
}

uint64_t get_maximum_blbi_count ()
{
    return bih_data.maximum_blbi_count;
}

uint64_t get_maximum_blbi_count_bytes ()
{
    return bih_data.maximum_blbi_count_bytes;
}
