// INCLUDE
#include "./interface.hpp"


// DATA
static struct {
    _COMPONENT* root = new _COMPONENT();
    map<string, _SHAPE*> shapes;
    map<string, _COMPONENT*> components;
    map<string, _LINE*> lines;
    uint32_t instance_count = 0;
} interface;


// CODE
_COMPONENT& get_root ()
{
    return *interface.root;
}

_COMPONENT* get_root_pointer ()
{
    return interface.root;
}

map<string, _SHAPE*>* get_shapes ()
{
    return &interface.shapes;
}

_SHAPE& get_shape (string id)
{
    if (interface.shapes.count(id) == 0) throw runtime_error("\nERROR: Shape id '" + id + "' not found.");
    return *get_shapes()->at(id);
}

void set_shape (_SHAPE* new_shape)
{
    interface.shapes[new_shape->id()] = new_shape;
}

map<string, _COMPONENT*>* get_components ()
{
    return &interface.components;
}

_COMPONENT& get_component (string id)
{
    if (interface.components.count(id) == 0) throw runtime_error("\nERROR: Component id '" + id + "' not found.");
    return *get_components()->at(id);
}

void set_component (_COMPONENT* new_component)
{
    interface.components[new_component->id()] = new_component;
}

map<string, _LINE*>* get_lines ()
{
    return &interface.lines;
}

_LINE& get_line (string id)
{
    if (interface.lines.count(id) == 0) throw runtime_error("\nERROR: Line id '" + id + "' not found.");
    return *get_lines()->at(id);
}

void set_line (_LINE* new_line)
{
    interface.lines[new_line->get_component().id()] = new_line;
}

uint32_t get_instance_count ()
{
    return interface.instance_count;
}

void increase_instance_count ()
{
    interface.instance_count++;
}
