// INCLUDE
#include "./component.hpp"
#include "../interface.hpp"
#include "../line/line.hpp"


// CODE
_COMPONENT::_COMPONENT ()
{
    if (get_root() != nullptr) throw runtime_error("\nERROR: No component id specified.");
    this->address = this;
    this->component_id = "root";
    this->component_spatial_state.depth = -1;
}

_COMPONENT::_COMPONENT (string id)
{
    if (get_components()->count(id) != 0) throw runtime_error("\nERROR: Component id '" + id + "' already exists.");
    this->address = this;
    this->component_id = id;
    this->parent = get_root();
    this->component_spatial_state.depth = get_components()->size();
    set_component(this);
}

_COMPONENT::~_COMPONENT ()
{
}

_COMPONENT& component (string id)
{
    return *(new _COMPONENT(id));
}

vec2 rotate (vec2 point, float degrees)
{
    float rad = radians(degrees);
    return mat2x2(cos(rad), -sin(rad), sin(rad), cos(rad)) * point;
}

_COMPONENT& _COMPONENT::add (_SHAPE* new_shape)
{
    if (this->parent->id() != "root") throw runtime_error("\nERROR: Shape addition should occur inline before component adoption");
    if (!new_shape->built()) throw runtime_error("\nERROR: The provided shape added to the component needs to be built first.");
    this->shapes.push_back( new_shape );
    int shape_index = this->shape_instances.size();
    this->shape_instances.push_back(create_instance(
        this->shapes.at(shape_index)->get_build(),
        this->component_spatial_state.position,
        this->component_spatial_state.scale,
        this->component_spatial_state.rotation
    ));
    increase_instance_count();
    this->shape_states.push_back({
        this->component_spatial_state.position,
        this->component_spatial_state.rotation,
        this->component_spatial_state.scale,
        get_instance_depth(this->shape_instances.at( this->shape_instances.size() - 1 ))
    });
    this->total_shape_count++;
    this->parent->total_shape_count++;
    return *this;
}

_COMPONENT& _COMPONENT::replace (int shape_index, _SHAPE* new_shape)
{
    if (shape_index >= this->shapes.size()) throw runtime_error("\ERROR: Shape index '" + to_string(shape_index) + "' does not exist.");
    if (!new_shape->built()) throw runtime_error("\nERROR: The provided shape inserted into the component needs to be built first.");
    update_instance_object( this->shape_instances.at(shape_index), new_shape->get_build() );
    return *this;
}

vec2 _COMPONENT::position ()
{
    return this->component_spatial_state.position;
}

vec2 _COMPONENT::position (int shape_index)
{
    return this->shape_states.at(shape_index).position;
}

_COMPONENT& _COMPONENT::position (vec2 coordinates)
{
    vec2 relative_change = coordinates - this->component_spatial_state.position;
    this->component_spatial_state.position = coordinates;

    for (int i = 0; i < this->shape_instances.size(); i++)
    {
        this->shape_states.at(i).position += relative_change;

        if (this->shape_instances.at(i) == nullptr) continue;

        update_instance_position(this->shape_instances.at(i), this->shape_states.at(i).position);
    }

    for (_COMPONENT* child : this->children)
    {
        child->component_spatial_state.position += relative_change;

        for (int i = 0; i < child->shape_instances.size(); i++)
        {
            child->shape_states.at(i).position += relative_change;

            if (child->shape_instances.at(i) == nullptr) continue;

            update_instance_position(child->shape_instances.at(i), child->shape_states.at(i).position);
        }
    }

    return *this;
}

_COMPONENT& _COMPONENT::position (int shape_index, vec2 coordinates)
{
    if (shape_index >= this->shapes.size()) throw runtime_error("\ERROR: Shape index '" + to_string(shape_index) + "' does not exist.");
    this->shape_states.at(shape_index).position = coordinates;
    update_instance_position(this->shape_instances.at(shape_index), coordinates);
    return *this;
}

_COMPONENT& _COMPONENT::x (float x_coordinate)
{
    float relative_change = x_coordinate - this->component_spatial_state.position.x;
    this->component_spatial_state.position.x = x_coordinate;

    for (int i = 0; i < this->shape_instances.size(); i++)
    {
        this->shape_states.at(i).position.x += relative_change;

        if (this->shape_instances.at(i) == nullptr) continue;

        update_instance_position(this->shape_instances.at(i), this->shape_states.at(i).position);
    }

    for (_COMPONENT* child : this->children)
    {
        child->component_spatial_state.position.x += relative_change;

        for (int i = 0; i < child->shape_instances.size(); i++)
        {
            child->shape_states.at(i).position.x += relative_change;

            if (child->shape_instances.at(i) == nullptr) continue;

            update_instance_position(child->shape_instances.at(i), child->shape_states.at(i).position);
        }
    }

    return *this;
}

_COMPONENT& _COMPONENT::x (int shape_index, float x_coordinate)
{
    if (shape_index >= this->shapes.size()) throw runtime_error("\ERROR: Shape index '" + to_string(shape_index) + "' does not exist.");
    this->shape_states.at(shape_index).position.x = x_coordinate;
    update_instance_position(this->shape_instances.at(shape_index), this->shape_states.at(shape_index).position);
    return *this;
}

_COMPONENT& _COMPONENT::y (float y_coordinate)
{
    float relative_change = y_coordinate - this->component_spatial_state.position.y;
    this->component_spatial_state.position.y = y_coordinate;

    for (int i = 0; i < this->shape_instances.size(); i++)
    {
        this->shape_states.at(i).position.y += relative_change;

        if (this->shape_instances.at(i) == nullptr) continue;

        update_instance_position(this->shape_instances.at(i), this->shape_states.at(i).position);
    }

    for (_COMPONENT* child : this->children)
    {
        child->component_spatial_state.position.y += relative_change;

        for (int i = 0; i < child->shape_instances.size(); i++)
        {
            child->shape_states.at(i).position.y += relative_change;

            if (child->shape_instances.at(i) == nullptr) continue;

            update_instance_position(child->shape_instances.at(i), child->shape_states.at(i).position);
        }
    }

    return *this;
}

_COMPONENT& _COMPONENT::y (int shape_index, float y_coordinate)
{
    if (shape_index >= this->shapes.size()) throw runtime_error("\ERROR: Shape index '" + to_string(shape_index) + "' does not exist.");
    this->shape_states.at(shape_index).position.y = y_coordinate;
    update_instance_position(this->shape_instances.at(shape_index), this->shape_states.at(shape_index).position);
    return *this;
}

float _COMPONENT::scale ()
{
    return this->component_spatial_state.scale;
}

float _COMPONENT::scale (int shape_index)
{
    return this->shape_states.at(shape_index).scale;
}

_COMPONENT& _COMPONENT::scale (float scale)
{
    if (scale < 0.001f) scale = 0.001f; 

    float old_scale = this->component_spatial_state.scale;
    this->component_spatial_state.scale = scale;

    for (int i = 0; i < this->shape_instances.size(); i++)
    {
        this->shape_states.at(i).position -= this->component_spatial_state.position;
        if (old_scale != 0.0) this->shape_states.at(i).position /= old_scale;
        this->shape_states.at(i).position *= this->component_spatial_state.scale;
        this->shape_states.at(i).position += this->component_spatial_state.position;

        if (old_scale != 0.0) this->shape_states.at(i).scale /= old_scale;
        this->shape_states.at(i).scale = this->component_spatial_state.scale * this->shape_states.at(i).scale;

        if (this->shape_instances.at(i) == nullptr) continue;

        update_instance_position(this->shape_instances.at(i), this->shape_states.at(i).position);
        update_instance_scale(this->shape_instances.at(i), this->shape_states.at(i).scale);
    }

    float original_child_scale;
    for (_COMPONENT* child : this->children)
    {
        child->component_spatial_state.position -= this->component_spatial_state.position;
        if (old_scale != 0.0) child->component_spatial_state.position /= old_scale;
        child->component_spatial_state.position *= this->component_spatial_state.scale;
        child->component_spatial_state.position += this->component_spatial_state.position;

        if (old_scale != 0.0) child->component_spatial_state.scale /= old_scale;
        original_child_scale = child->component_spatial_state.scale;
        child->component_spatial_state.scale = this->component_spatial_state.scale * child->component_spatial_state.scale;

        for (int i = 0; i < child->shape_instances.size(); i++)
        {
            child->shape_states.at(i).position -= this->component_spatial_state.position;

            if (old_scale != 0.0) child->shape_states.at(i).position /= old_scale;
            if (original_child_scale != 0.0) child->shape_states.at(i).position /= original_child_scale;

            child->shape_states.at(i).position *= this->component_spatial_state.scale * original_child_scale;
            child->shape_states.at(i).position += this->component_spatial_state.position;

            if (old_scale != 0.0) child->shape_states.at(i).scale /= old_scale;
            if (original_child_scale != 0.0) child->shape_states.at(i).scale /= original_child_scale;
            child->shape_states.at(i).scale = child->component_spatial_state.scale * child->shape_states.at(i).scale;

            if (child->shape_instances.at(i) == nullptr) continue;

            update_instance_position(child->shape_instances.at(i), child->shape_states.at(i).position);
            update_instance_scale(child->shape_instances.at(i), child->shape_states.at(i).scale);
        }
    }

    return *this;
}

_COMPONENT& _COMPONENT::scale (int shape_index, float scale)
{
    if (scale < 0.001f) scale = 0.001f; 
    if (shape_index >= this->shapes.size()) throw runtime_error("\ERROR: Shape index '" + to_string(shape_index) + "' does not exist.");

    this->shape_states.at(shape_index).scale = this->component_spatial_state.scale * scale;
    update_instance_scale(this->shape_instances.at(shape_index), this->shape_states.at(shape_index).scale);

    return *this;
}

float _COMPONENT::rotation ()
{
    return this->component_spatial_state.rotation;
}

float _COMPONENT::rotation (int shape_index)
{
    return this->shape_states.at(shape_index).rotation;
}

_COMPONENT& _COMPONENT::rotation (float angle)
{
    float old_rotation = this->component_spatial_state.rotation;
    this->component_spatial_state.rotation = angle;

    for (int i = 0; i < this->shape_instances.size(); i++)
    {
        this->shape_states.at(i).rotation = (this->shape_states.at(i).rotation - old_rotation) + angle;

        this->shape_states.at(i).position = rotate(this->shape_states.at(i).position - this->component_spatial_state.position, -1.0f * (angle - old_rotation)) + this->component_spatial_state.position;

        if (this->shape_instances.at(i) == nullptr) continue;

        update_instance_position(this->shape_instances.at(i), this->shape_states.at(i).position);
        update_instance_rotation(this->shape_instances.at(i), this->shape_states.at(i).rotation);
    }

    for (_COMPONENT* child : this->children)
    {
        child->component_spatial_state.rotation = (child->component_spatial_state.rotation - old_rotation) + angle;
        child->component_spatial_state.position = rotate(child->component_spatial_state.position - this->component_spatial_state.position, -1.0f * (angle - old_rotation)) + this->component_spatial_state.position;

        for (int i = 0; i < child->shape_instances.size(); i++)
        {
            child->shape_states.at(i).rotation = (child->shape_states.at(i).rotation - old_rotation) + angle;
            child->shape_states.at(i).position = rotate(child->shape_states.at(i).position - this->component_spatial_state.position, -1.0f * (angle - old_rotation)) + this->component_spatial_state.position;

            if (child->shape_instances.at(i) == nullptr) continue;

            update_instance_position(child->shape_instances.at(i), child->shape_states.at(i).position);
            update_instance_rotation(child->shape_instances.at(i), child->shape_states.at(i).rotation);
        }
    }

    return *this;
}

_COMPONENT& _COMPONENT::rotation (int shape_index, float angle)
{
    if (shape_index >= this->shapes.size()) throw runtime_error("\ERROR: Shape index '" + to_string(shape_index) + "' does not exist.");
    this->shape_states.at(shape_index).rotation = this->component_spatial_state.rotation + angle;
    update_instance_rotation(this->shape_instances.at(shape_index), this->shape_states.at(shape_index).rotation);
    return *this;
}

_COMPONENT& _COMPONENT::front ()
{
    bool is_child = this->parent->id() != "root";

    uint32_t new_depth = is_child ? this->parent->children.at( this->parent->children.size() - 1 )->component_spatial_state.depth : get_components()->size() - this->children.size() - 1;
    uint32_t old_depth = this->component_spatial_state.depth;
    uint32_t depth_offset = this->children.size() + 1;

    uint32_t moved_shape_count = 0;
    uint32_t component_depth;
    if (is_child)
    {
        _COMPONENT* child;
        for (int child_index = 0; child_index < this->parent->children.size(); child_index++)
        {
            child = this->parent->children.at(child_index);

            if (child->id() == this->id()) continue;

            component_depth = child->component_spatial_state.depth;
            child->component_spatial_state.depth -= component_depth > old_depth ? depth_offset : 0;

            if (component_depth > old_depth)
            {
                moved_shape_count += child->shapes.size();

                for (int i = 0; i < child->shape_states.size(); i++)
                {
                    child->shape_states.at(i).depth -= this->total_shape_count;
                    set_instance_depth(child->shape_instances.at(i), child->shape_states.at(i).depth);
                }
            }
        }
    }
    else
    {
        map<string, _COMPONENT*>::iterator component_iterator = get_components()->begin();

        while (component_iterator != get_components()->end())
        {
            if (component_iterator->second->parent == this || component_iterator->first == this->id())
            {
                component_iterator++;
                continue;
            }

            component_depth = component_iterator->second->component_spatial_state.depth;
            component_iterator->second->component_spatial_state.depth -= component_depth > old_depth ? depth_offset : 0;

            if (component_depth > old_depth)
            {
                moved_shape_count += component_iterator->second->shapes.size();

                for (int i = 0; i < component_iterator->second->shape_states.size(); i++)
                {
                    component_iterator->second->shape_states.at(i).depth -= this->total_shape_count;
                    set_instance_depth(component_iterator->second->shape_instances.at(i), component_iterator->second->shape_states.at(i).depth);
                }
            }

            component_iterator++;
        }
    }

    this->component_spatial_state.depth = new_depth;
    for (int shape_index = 0; shape_index < this->shape_states.size(); shape_index++)
    {
        this->shape_states.at(shape_index).depth += moved_shape_count;
        set_instance_depth(this->shape_instances.at(shape_index), this->shape_states.at(shape_index).depth);
    }

    if (is_child) return *this;

    _COMPONENT* child;
    for (int i = 0; i < this->children.size(); i++)
    {
        child = this->children.at(i);
        child->component_spatial_state.depth += (new_depth - old_depth);

        for (int shape_index = 0; shape_index < child->shape_states.size(); shape_index++)
        {
            child->shape_states.at(shape_index).depth += moved_shape_count;
            set_instance_depth(child->shape_instances.at(shape_index), child->shape_states.at(shape_index).depth);
        }
    }
    
    return *this;
}

_COMPONENT& _COMPONENT::state (string key, any value)
{
    this->component_state[key] = value;
    return *this;
}

any _COMPONENT::state (string key)
{
    return this->component_state[key];
}

_COMPONENT& _COMPONENT::adopt (_COMPONENT* new_child)
{
    if (new_child->parent != get_root()) throw runtime_error("\nERROR: Child component already assigned to a parent while attempting adoption.");
    if (new_child->children.size() > 0) throw runtime_error("\nERROR: Child component '" + new_child->id() + "' has children of its own. More than two component levels (e.g. grand-child components) are currently not possible.");

    this->children.push_back(new_child);
    new_child->parent = this;

    new_child->position( rotate(
        new_child->component_spatial_state.position * new_child->component_spatial_state.scale,
        -1.0f * this->component_spatial_state.rotation
    ) + this->component_spatial_state.position );
    new_child->rotation( this->component_spatial_state.rotation );
    new_child->scale( this->component_spatial_state.scale * new_child->component_spatial_state.scale );
    new_child->front();

    this->total_shape_count += new_child->shapes.size();

    return *this;
}

_COMPONENT& _COMPONENT::expel (int child_index)
{
    if (child_index >= this->children.size()) throw runtime_error("\nERROR: Child index'" + to_string(child_index) + "' does not exist while attempting expulsion of child.");
    this->total_shape_count -= this->children.at(child_index)->shapes.size();
    this->children.at(child_index)->parent = get_root();
    this->children.erase(this->children.begin() + child_index);
    return *this;
}

string _COMPONENT::id ()
{
    return this->component_id;
}

void _COMPONENT::print ()
{
    cout << endl << this->address << "\nid: " << this->id() << "\nparent: " << this->parent->id() << "\nposition: " << this->component_spatial_state.position.x << ", " << this->component_spatial_state.position.y << "\nrotation: " << this->component_spatial_state.rotation << "\nscale: " << this->component_spatial_state.scale << "\ndepth: " << this->component_spatial_state.depth << "\nstates: ";

    if (this->component_state.size() == 0) cout << "none" << endl;
    else 
    {
        cout << endl;

        map<string, any>::iterator iterator = this->component_state.begin();
        string value_type;
        while (iterator != this->component_state.end())
        {
            cout << "    " << iterator->first << ": ";

            value_type = iterator->second.type().name();

            if (value_type == "i") cout << any_cast<int>(iterator->second) << endl;
            else if (value_type == "d") cout << any_cast<double>(iterator->second) << endl;
            else if (value_type == "f") cout << any_cast<float>(iterator->second) << endl;
            else if (value_type == "PKc") cout << any_cast<const char*>(iterator->second) << endl;
            else if (value_type == "P9COMPONENT") cout << any_cast<_COMPONENT*>(iterator->second) << endl;
            else if (value_type == "P5_LINE") cout << any_cast<_LINE*>(iterator->second) << endl;
            else if (value_type == "N3glm3vecILi2EfLNS_9qualifierE0EEE") cout << any_cast<vec2>(iterator->second).x << ", " << any_cast<vec2>(iterator->second).y << endl;
            else cout << iterator->second.type().name() << endl;

            iterator++;
        }
    }

    cout << "cumulative shape count: " << this->total_shape_count << endl;
    cout << "shape states: " << endl;
    int index = 0;
    for (SPATIAL_STATE state : this->shape_states)
    {
        cout << index << ": " << this->shapes.at(index)->id() << endl;
        cout << "    position: " << state.position.x << ", " << state.position.y << endl; 
        cout << "    rotation: " << state.rotation << endl; 
        cout << "    scale: " << state.scale << endl;
        cout << "    depth: " << state.depth << endl;
        index++;
    }

    cout << "children: ";
    if (this->children.size() == 0) cout << "none" << endl;
    else
    {
        cout << endl;
        for (_COMPONENT* child : this->children) cout << "    " << child->address << endl;
    }

    cout << endl;
}

void _COMPONENT::delete_component ()
{
    get_components()->erase(this->id());
    delete this->address;
}
