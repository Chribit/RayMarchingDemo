// INCLUDE
#include "./object.hpp"
#include "../primitive/primitive.hpp"


// CODE
OBJECT create_object (vector< vector<int> > primitive_csg_hierarchies)
{
    OBJECT new_object = new object;
    new_object->primitive_range = determine_primitive_range( primitive_csg_hierarchies );

    indexed_aabb iaabb;
    int offset = get_primitives()->size() - get_csg_hierarchies_size( primitive_csg_hierarchies );
    for (int i = 0; i < primitive_csg_hierarchies.size(); i++)
    {
        iaabb = determine_csg_hierarchy_aabb( primitive_csg_hierarchies[i], offset, i );
        new_object->primitive_aabbs.push_back( iaabb );
        offset += primitive_csg_hierarchies[i].size();
    }

    determine_object_aabb( new_object );
    construct_blbih( new_object );
    store_object_pointer( new_object );
    return new_object;
}

void delete_object (OBJECT target_object, bool cleanup)
{
    if (!cleanup)
    {
        delete_object_instances(target_object);
        delete_object_primitives(target_object);
        delete_object_blbih(target_object);

        get_object_pointers()->erase( get_object_pointers()->begin() + target_object->index );

        for (int i = target_object->index; i < get_object_pointers()->size(); i++)
        {
            get_object_pointers()->at(i)->index--;
        }
    }

    delete target_object;
}

void cleanup_objects ()
{
    for (int index = 0; index < get_object_pointers()->size(); index++)
    {
        delete_object( get_object_pointers()->at(index), true );
    }
}
