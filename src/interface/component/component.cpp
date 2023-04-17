// INCLUDE
#include "./component.hpp"
#include "../interface.hpp"
#include "../line/line.hpp"
#include "../event/event.hpp"


// CODE
_COMPONENT::_COMPONENT ()
{
    if (get_root_pointer() != nullptr) throw runtime_error("\nERROR: No component id specified.");
    this->address = this;
    this->component_id = "root";
    this->component_spatial_state.depth = 0;
    this->parent = this;
    this->state("instance", new uint32_t(0xFFFFFFFF));
}

_COMPONENT::_COMPONENT (string id)
{
    if (get_components()->count(id) != 0) throw runtime_error("\nERROR: Component id '" + id + "' already exists.");
    this->address = this;
    this->component_id = id;
    set_component(this);
    get_root().adopt(this);
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

void propagate_shape_count (_COMPONENT* origin, uint32_t shape_count)
{
    _COMPONENT* current = origin; 
    bool is_root = false;
    while (!is_root)
    {
        current->add_total_shape_count(shape_count);
        is_root = current->id() == "root";
        current = current->get_parent();
    }
}

void propagate_child_count (_COMPONENT* origin, uint32_t child_count)
{
    _COMPONENT* current = origin; 
    bool is_root = false;
    while (!is_root)
    {
        current->add_total_child_count(child_count); 
        is_root = current->id() == "root";
        current = current->get_parent();
    }
}

_COMPONENT& _COMPONENT::add (_SHAPE* new_shape)
{
    if (this->parent->id() != "root") throw runtime_error("\nERROR: Shape addition should occur inline before component adoption");
    if (!new_shape->built()) throw runtime_error("\nERROR: The provided shape added to the component needs to be built first.");
    if (this->has_hovering) throw runtime_error("\nERROR: A component with active hovering effects can not recieve any more shapes.");
    this->shapes.push_back( new_shape );
    int shape_index = this->shape_instances.size();
    this->shape_instances.push_back(create_instance(
        this->shapes.at(shape_index)->get_build(),
        this->component_spatial_state.position,
        this->component_spatial_state.scale,
        this->component_spatial_state.rotation
    ));
    increase_instance_count();
    register_component(this->shape_instances.back(), this->address);
    this->shape_states.push_back({
        this->component_spatial_state.position,
        this->component_spatial_state.rotation,
        this->component_spatial_state.scale,
        get_instance_depth(this->shape_instances.back())
    });
    propagate_shape_count(this, 1);
    return *this;
}

_COMPONENT& _COMPONENT::replace (int shape_index, _SHAPE* new_shape)
{
    if (shape_index >= this->shapes.size()) throw runtime_error("\nERROR: Shape index '" + to_string(shape_index) + "' does not exist.");
    if (!new_shape->built()) throw runtime_error("\nERROR: The provided shape inserted into the component needs to be built first.");
    if (this->has_hovering) throw runtime_error("\nERROR: A component with active hovering effects can not modify its shapes.");
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

void translate_shapes (_COMPONENT* origin, vec2 relative_change)
{
    for (int i = 0; i < origin->get_shape_instances()->size(); i++)
    {
        origin->get_shape_states()->at(i).position += relative_change;

        if (origin->get_shape_instances()->at(i) == nullptr) continue;

        update_instance_position(origin->get_shape_instances()->at(i), origin->get_shape_states()->at(i).position);
    }
}

void translate_children (_COMPONENT* origin, vec2 relative_change)
{
    for (_COMPONENT* child : *origin->get_children())
    {
        child->get_component_spatial_state()->position += relative_change;

        translate_shapes(child, relative_change);

        translate_children(child, relative_change);
    }
}

_COMPONENT& _COMPONENT::position (vec2 coordinates)
{
    vec2 relative_change = coordinates - this->component_spatial_state.position;
    this->component_spatial_state.position = coordinates;

    translate_shapes(this, relative_change);
    translate_children(this, relative_change);

    return *this;
}

_COMPONENT& _COMPONENT::position (int shape_index, vec2 coordinates)
{
    if (shape_index >= this->shapes.size()) throw runtime_error("\nERROR: Shape index '" + to_string(shape_index) + "' does not exist.");
    this->shape_states.at(shape_index).position = coordinates;
    update_instance_position(this->shape_instances.at(shape_index), coordinates);
    return *this;
}

_COMPONENT& _COMPONENT::x (float x_coordinate)
{
    vec2 relative_change = vec2(x_coordinate - this->component_spatial_state.position.x, 0.0f);
    this->component_spatial_state.position.x = x_coordinate;

    translate_shapes(this, relative_change);
    translate_children(this, relative_change);

    return *this;
}

_COMPONENT& _COMPONENT::x (int shape_index, float x_coordinate)
{
    if (shape_index >= this->shapes.size()) throw runtime_error("\nERROR: Shape index '" + to_string(shape_index) + "' does not exist.");
    this->shape_states.at(shape_index).position.x = x_coordinate;
    update_instance_position(this->shape_instances.at(shape_index), this->shape_states.at(shape_index).position);
    return *this;
}

_COMPONENT& _COMPONENT::y (float y_coordinate)
{
    vec2 relative_change = vec2(0.0f, y_coordinate - this->component_spatial_state.position.y);
    this->component_spatial_state.position.y = y_coordinate;

    translate_shapes(this, relative_change);
    translate_children(this, relative_change);

    return *this;
}

_COMPONENT& _COMPONENT::y (int shape_index, float y_coordinate)
{
    if (shape_index >= this->shapes.size()) throw runtime_error("\nERROR: Shape index '" + to_string(shape_index) + "' does not exist.");
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

vec2 scale_position (vec2 current_position, vec2 origin_position, float old_scaling, float new_scaling)
{
    vec2 new_position = current_position;
    new_position -= origin_position;
    new_position /= old_scaling;
    new_position *= new_scaling;
    new_position += origin_position;
    return new_position;
}

float scale_scaling (float current_scaling, float old_scaling, float new_scaling)
{
    float new_scale = current_scaling;
    new_scale /= old_scaling;
    new_scale *= new_scaling;
    return new_scale;
}

void scale_shapes (_COMPONENT* owner, vec2 origin_position, float old_scale, float new_scale)
{
    for (int i = 0; i < owner->get_shape_instances()->size(); i++)
    {
        owner->get_shape_states()->at(i).position = scale_position(
            owner->get_shape_states()->at(i).position,
            origin_position,
            old_scale,
            new_scale
        );

        owner->get_shape_states()->at(i).scale = scale_scaling(
            owner->get_shape_states()->at(i).scale,
            old_scale,
            new_scale
        );

        update_instance_position(owner->get_shape_instances()->at(i), owner->get_shape_states()->at(i).position);
        update_instance_scale(owner->get_shape_instances()->at(i), owner->get_shape_states()->at(i).scale);
    }
}

void scale_children (_COMPONENT* owner, vec2 origin_position, float old_scale, float new_scale)
{
    float old_child_scale;
    for (_COMPONENT* child : *owner->get_children())
    {
        child->get_component_spatial_state()->position = scale_position(
            child->get_component_spatial_state()->position,
            origin_position,
            old_scale,
            new_scale
        );

        child->get_component_spatial_state()->scale /= old_scale;
        old_child_scale = child->get_component_spatial_state()->scale;
        child->get_component_spatial_state()->scale *= new_scale;

        scale_shapes(child, origin_position, old_scale * old_child_scale, child->get_component_spatial_state()->scale);
        scale_children(child, origin_position, old_scale * old_child_scale, child->get_component_spatial_state()->scale);
    }
}

_COMPONENT& _COMPONENT::scale (float scale)
{
    if (scale < 0.001f) scale = 0.001f; 

    float old_scale = this->component_spatial_state.scale;
    this->component_spatial_state.scale = scale;

    scale_shapes(this, this->component_spatial_state.position, old_scale, scale);
    scale_children(this, this->component_spatial_state.position, old_scale, scale);

    return *this;
}

_COMPONENT& _COMPONENT::scale (int shape_index, float scale)
{
    if (scale < 0.001f) scale = 0.001f; 
    if (shape_index >= this->shapes.size()) throw runtime_error("\nERROR: Shape index '" + to_string(shape_index) + "' does not exist.");

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

void rotate_shapes (_COMPONENT* target, float old_angle, float angle, vec2 rotation_origin_transform)
{
    for (int i = 0; i < target->get_shape_instances()->size(); i++)
    {
        target->get_shape_states()->at(i).rotation = (target->get_shape_states()->at(i).rotation - old_angle) + angle;

        target->get_shape_states()->at(i).position = rotate(
                target->get_shape_states()->at(i).position - rotation_origin_transform,
                -1.0f * (angle - old_angle)
            ) + rotation_origin_transform;

        if (target->get_shape_instances()->at(i) == nullptr) continue;
        update_instance_position(target->get_shape_instances()->at(i), target->get_shape_states()->at(i).position);
        update_instance_rotation(target->get_shape_instances()->at(i), target->get_shape_states()->at(i).rotation);
    }
}

void rotate_children (_COMPONENT* target, float old_angle, float angle, vec2 rotation_origin_transform)
{
    for (_COMPONENT* child : *target->get_children())
    {
        child->get_component_spatial_state()->rotation = (child->get_component_spatial_state()->rotation - old_angle) + angle;

        child->get_component_spatial_state()->position = rotate(
                child->get_component_spatial_state()->position - rotation_origin_transform, 
                -1.0f * (angle - old_angle)
            ) + rotation_origin_transform;

        rotate_shapes(child, old_angle, angle, rotation_origin_transform);

        rotate_children(child, old_angle, angle, rotation_origin_transform);
    }
}

_COMPONENT& _COMPONENT::rotation (float angle)
{
    float old_angle = this->component_spatial_state.rotation;
    this->component_spatial_state.rotation = angle;

    rotate_shapes(this, old_angle, angle, this->component_spatial_state.position);

    rotate_children(this, old_angle, angle, this->component_spatial_state.position);

    return *this;
}

_COMPONENT& _COMPONENT::rotation (int shape_index, float angle)
{
    if (shape_index >= this->shapes.size()) throw runtime_error("\nERROR: Shape index '" + to_string(shape_index) + "' does not exist.");
    this->shape_states.at(shape_index).rotation = this->component_spatial_state.rotation + angle;
    update_instance_rotation(this->shape_instances.at(shape_index), this->shape_states.at(shape_index).rotation);
    return *this;
}

void propagate_depth_change (_COMPONENT* origin, uint32_t summand)
{
    for (_COMPONENT* child : *origin->get_children())
    {
        for (int i = 0; i < child->get_shapes()->size(); i++)
        {
            child->get_shape_states()->at(i).depth += summand;
            set_instance_depth(child->get_shape_instances()->at(i), child->get_shape_states()->at(i).depth);
        }
        propagate_depth_change(child, summand);
    }
}

_COMPONENT& _COMPONENT::front ()
{
    if (this->id() == "root") throw runtime_error("\nERROR: The root component's depth can not change.");

    uint32_t old_depth = this->component_spatial_state.depth;
    uint32_t moved_shapes = 0;

    for (_COMPONENT* sibling : *this->parent->get_children())
    {
        if (sibling->component_spatial_state.depth > old_depth)
        {
            sibling->component_spatial_state.depth--;
            for (int i = 0; i < sibling->get_shapes()->size(); i++)
            {
                sibling->get_shape_states()->at(i).depth -= this->total_shape_count;
                set_instance_depth(sibling->get_shape_instances()->at(i), sibling->get_shape_states()->at(i).depth);
            }
            moved_shapes += sibling->get_shapes()->size();
        }
    }

    this->component_spatial_state.depth = this->parent->get_children()->size() - 1;
    for (int i = this->get_shapes()->size() - 1; i >= 0; i--)
    {
        this->get_shape_states()->at(i).depth += moved_shapes;
        set_instance_depth(this->get_shape_instances()->at(i), this->get_shape_states()->at(i).depth);
    }
    propagate_depth_change(this, moved_shapes);
    
    return *this;
}

_COMPONENT& _COMPONENT::state (string key, any value)
{
    if (strcmp(value.type().name(), "PKc") == 0) value = string(any_cast<const char*>(value));
    if (strcmp(value.type().name(), "10_COMPONENT") == 0) value = get_components()->at(any_cast<_COMPONENT&>(value).id());
    if (strcmp(value.type().name(), "5_LINE") == 0) value = get_lines()->at(any_cast<_LINE&>(value).id());
    string event_name = "react-" + this->id() + "-" + key;
    if (is_event_registered(event_name))
    {
        vector<string> reactors = this->component_state_reactors[key];
        for (int i = 0; i < reactors.size(); i++)
        {
            broadcast_event(get_component(reactors[i]), event_name, {
                {"new value", value},
                {"old value", this->component_state[key]}
            });
        }
    }
    this->component_state[key] = value;
    return *this;
}

any _COMPONENT::state (string key)
{
    return this->component_state[key];
}

_COMPONENT& _COMPONENT::react (string component_id, string state_name, function<void(event*)> new_callback)
{
    string event_name = "react-" + component_id + "-" + state_name;
    register_event(event_name, true);
    this->component_event_callbacks[event_name].push_back(new_callback);
    get_component(component_id).set_state_reactor(state_name, this->id());
    return *this;
}

_COMPONENT& _COMPONENT::adopt (_COMPONENT* new_child)
{
    if (new_child->parent != nullptr) new_child->parent->expel(new_child);

    new_child->parent_child_index = this->children.size();
    this->children.push_back(new_child);
    new_child->parent = this;

    if (this->id() != "root")
    {
        new_child->position( rotate(
            new_child->component_spatial_state.position * new_child->component_spatial_state.scale,
            -1.0f * this->component_spatial_state.rotation
        ) + this->component_spatial_state.position );
        new_child->rotation( this->component_spatial_state.rotation );
        new_child->scale( this->component_spatial_state.scale * new_child->component_spatial_state.scale );
    }

    new_child->front();

    propagate_shape_count(this, new_child->total_shape_count);
    propagate_child_count(this, new_child->total_child_count + 1);

    return *this;
}

_COMPONENT& _COMPONENT::expel (_COMPONENT* child)
{
    if (child->parent != this) throw runtime_error("\nERROR: Component with id '" + child->id() + "' is not a child of component with id '" + this->id() + "'.");

    child->front();

    propagate_shape_count(this, -1 * child->total_shape_count);
    propagate_child_count(this, -1 * (child->total_child_count + 1));

    child->parent = nullptr;

    this->children.erase(this->children.begin() + child->parent_child_index);
    return *this;
}

_COMPONENT& _COMPONENT::listen (string registered_event, function<void(event*)> new_callback)
{
    if (!is_event_registered(registered_event)) throw runtime_error("\nERROR: The event '" + registered_event + "' is not registered and can therefor not be listened to.");
    this->component_event_callbacks[registered_event].push_back(new_callback);
    return *this;
}

_COMPONENT& _COMPONENT::ignore (string registered_event, int callback_index)
{
    if (!is_event_registered(registered_event)) throw runtime_error("\nERROR: The event '" + registered_event + "' is not registered and can therefor not be ignored.");
    this->component_event_callbacks[registered_event].erase( this->component_event_callbacks[registered_event].begin() + callback_index );
    return *this;
}

_COMPONENT& _COMPONENT::hover (vector<colour> hover_colours)
{
    if (hover_colours.size() > this->shapes.size()) throw runtime_error("\nERROR: There can not be more colours for hovering than shapes in a component. Provided " + to_string(hover_colours.size()) + " colours for the component '" + this->id() + "' with " + to_string(this->shapes.size()) + " shape" + (this->shapes.size() > 1 ? "s." : "."));
    if (this->has_hovering) throw runtime_error("\nERROR: A component can only have one hover effect. Attempted to add additional hover effects to component '" + this->id() + "'.");

    for (int i = 0; i < hover_colours.size(); i++)
    {
        SHAPE_DATA shape_data = this->shapes[i]->data();
        if (shape_data.multi_shape) continue;
        this->add(
            &shape(this->shapes[i]->id() + "-hover")
                .radius(shape_data.radius_data)
                .width(shape_data.dimension_data.x)
                .height(shape_data.dimension_data.y)
                .x(shape_data.position_data.x)
                .y(shape_data.position_data.y)
                .thickness(shape_data.thickness_data)
                .rotate(shape_data.rotation_data)
                .repeat(shape_data.x_repetition, shape_data.y_repetition)
                .red(hover_colours[i].red)
                .green(hover_colours[i].green)
                .blue(hover_colours[i].blue)
                .visible(false)
                .build()
        );
    }

    this->hovering_callbacks = uvec2(this->component_event_callbacks["mouse enter"].size(), this->component_event_callbacks["mouse leave"].size());

    this->listen("mouse enter", [](event* data){
        string id = get_string(data, "component id");
        int start = get_component(id).get_shapes()->size() / 2;
        for (int i = 0; i < start; i++)
        {
            update_primitive_colour(
                get_component(id).get_shapes()->at(i)->get_build(),
                0,
                {0, 0, 0, 0}
            );
        }
        for (int i = start; i < get_component(id).get_shapes()->size(); i++)
        {
            update_primitive_colour(
                get_component(id).get_shapes()->at(i)->get_build(),
                0,
                {
                    .red = get_component(id).get_shapes()->at(i)->data().colour_data.red,
                    .green = get_component(id).get_shapes()->at(i)->data().colour_data.green,
                    .blue = get_component(id).get_shapes()->at(i)->data().colour_data.blue,
                    255
                }
            );
        }
    });
    this->listen("mouse leave", [](event* data){
        string id = get_string(data, "component id");
        int start = get_component(id).get_shapes()->size() / 2;
        for (int i = 0; i < start; i++)
        {
            update_primitive_colour(
                get_component(id).get_shapes()->at(i)->get_build(),
                0,
                {
                    .red = get_component(id).get_shapes()->at(i)->data().colour_data.red,
                    .green = get_component(id).get_shapes()->at(i)->data().colour_data.green,
                    .blue = get_component(id).get_shapes()->at(i)->data().colour_data.blue,
                    255
                }
            );
        }
        for (int i = start; i < get_component(id).get_shapes()->size(); i++)
        {
            update_primitive_colour(
                get_component(id).get_shapes()->at(i)->get_build(),
                0,
                {0, 0, 0, 0}
            );
        }
    });

    this->has_hovering = true;

    return *this;
}

void _COMPONENT::stop_hover ()
{
    if (!this->has_hovering) throw runtime_error("\nERROR: Attempted hover effect removal on component '" + this->id() + "' which has no hovering effect.");

    int start = this->get_shapes()->size() / 2;
    for (int i = start; i < this->get_shapes()->size(); i++)
    {
        this->shapes.at(start)->delete_shape();
        this->shapes.erase(this->shapes.begin() + start);
        this->shape_states.erase(this->shape_states.begin() + start);
        this->shape_instances.erase(this->shape_instances.begin() + start);
    }

    this->ignore("mouse enter", this->hovering_callbacks.x);
    this->ignore("mouse leave", this->hovering_callbacks.y);

    if (this->component_event_callbacks["mouse enter"].size() == 0) this->component_event_callbacks.erase("mouse enter");
    if (this->component_event_callbacks["mouse leave"].size() == 0) this->component_event_callbacks.erase("mouse leave");
}

bool _COMPONENT::get_hovering ()
{
    return this->has_hovering;
}

SPATIAL_STATE* _COMPONENT::get_component_spatial_state ()
{
    return &this->component_spatial_state;
}

void _COMPONENT::add_total_shape_count (uint32_t summand)
{
    this->total_shape_count += summand;
}

void _COMPONENT::add_total_child_count (uint32_t summand)
{
    this->total_child_count += summand;
}

_COMPONENT* _COMPONENT::get_parent ()
{
    return this->parent;
}

vector<_COMPONENT*>* _COMPONENT::get_children ()
{
    return &this->children;
}

vector<_SHAPE*>* _COMPONENT::get_shapes ()
{
    return &this->shapes;
}

vector<OBJECT_INSTANCE>* _COMPONENT::get_shape_instances ()
{
    return &this->shape_instances;
}

vector<SPATIAL_STATE>* _COMPONENT::get_shape_states ()
{
    return &this->shape_states;
}

void _COMPONENT::run_callbacks (string registered_event, event* event_data)
{
    for (int i = 0; i < this->component_event_callbacks[registered_event].size(); i++)
    {
        this->component_event_callbacks[registered_event].at(i)(event_data);
    }
}

void _COMPONENT::set_state_reactor (string state_name, string reactor_id)
{
    this->component_state_reactors[state_name].push_back(reactor_id);
}

map<string, vector<function<void(event*)>>>* _COMPONENT::get_component_event_callbacks ()
{
    return &this->component_event_callbacks;
}

string _COMPONENT::id ()
{
    return this->component_id;
}

void _COMPONENT::print ()
{
    bool is_root = this->id() == "root";

    cout << endl << this->address << "\nid: " << this->id() << (is_root ? "" : "\nparent: ") << (is_root ? "" : this->parent->id()) << "\nposition: " << this->component_spatial_state.position.x << ", " << this->component_spatial_state.position.y << "\nrotation: " << this->component_spatial_state.rotation << "\nscale: " << this->component_spatial_state.scale << "\ndepth: " << this->component_spatial_state.depth << "\nstates: ";

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

            if (value_type == "b") cout << (any_cast<bool>(iterator->second) ? "true" : "false" ) << endl;
            else if (value_type == "i") cout << any_cast<int>(iterator->second) << endl;
            else if (value_type == "j") cout << any_cast<uint32_t>(iterator->second) << endl;
            else if (value_type == "Pj") cout << any_cast<uint32_t*>(iterator->second) << endl;
            else if (value_type == "d") cout << any_cast<double>(iterator->second) << endl;
            else if (value_type == "f") cout << any_cast<float>(iterator->second) << endl;
            else if (value_type == "NSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE") cout << any_cast<string>(iterator->second) << endl;
            else if (value_type == "N3glm3vecILi2EfLNS_9qualifierE0EEE") cout << any_cast<vec2>(iterator->second).x << ", " << any_cast<vec2>(iterator->second).y << endl;
            else if (value_type == "N3glm3vecILi2EjLNS_9qualifierE0EEE") cout << any_cast<uvec2>(iterator->second).x << ", " << any_cast<uvec2>(iterator->second).y << endl;
            else if (value_type == "N3glm3vecILi2EdLNS_9qualifierE0EEE") cout << any_cast<dvec2>(iterator->second).x << ", " << any_cast<dvec2>(iterator->second).y << endl;
            else if (value_type == "N3glm3vecILi2EbLNS_9qualifierE0EEE") cout << any_cast<bvec2>(iterator->second).x << ", " << any_cast<bvec2>(iterator->second).y << endl;
            else if (value_type == "N3glm3vecILi3EfLNS_9qualifierE0EEE") cout << any_cast<vec3>(iterator->second).x << ", " << any_cast<vec3>(iterator->second).y << ", " << any_cast<vec3>(iterator->second).z << endl;
            else if (value_type == "N3glm3vecILi3EjLNS_9qualifierE0EEE") cout << any_cast<uvec3>(iterator->second).x << ", " << any_cast<uvec3>(iterator->second).y << ", " << any_cast<uvec3>(iterator->second).z << endl;
            else if (value_type == "N3glm3vecILi3EdLNS_9qualifierE0EEE") cout << any_cast<dvec3>(iterator->second).x << ", " << any_cast<dvec3>(iterator->second).y << ", " << any_cast<dvec3>(iterator->second).z << endl;
            else if (value_type == "N3glm3vecILi3EbLNS_9qualifierE0EEE") cout << any_cast<bvec3>(iterator->second).x << ", " << any_cast<bvec3>(iterator->second).y << ", " << any_cast<bvec3>(iterator->second).z << endl;
            else if (value_type == "N3glm3vecILi4EfLNS_9qualifierE0EEE") cout << any_cast<vec4>(iterator->second).x << ", " << any_cast<vec4>(iterator->second).y << ", " << any_cast<vec4>(iterator->second).z << ", " << any_cast<vec4>(iterator->second).w << endl;
            else if (value_type == "N3glm3vecILi4EjLNS_9qualifierE0EEE") cout << any_cast<uvec4>(iterator->second).x << ", " << any_cast<uvec4>(iterator->second).y << ", " << any_cast<uvec4>(iterator->second).z << ", " << any_cast<uvec4>(iterator->second).w << endl;
            else if (value_type == "N3glm3vecILi4EdLNS_9qualifierE0EEE") cout << any_cast<dvec4>(iterator->second).x << ", " << any_cast<dvec4>(iterator->second).y << ", " << any_cast<dvec4>(iterator->second).z << ", " << any_cast<dvec4>(iterator->second).w << endl;
            else if (value_type == "N3glm3vecILi4EbLNS_9qualifierE0EEE") cout << any_cast<bvec4>(iterator->second).x << ", " << any_cast<bvec4>(iterator->second).y << ", " << any_cast<bvec4>(iterator->second).z << ", " << any_cast<bvec4>(iterator->second).w << endl;
            else if (value_type == "P10_COMPONENT") cout << any_cast<_COMPONENT*>(iterator->second) << endl;
            else if (value_type == "P5_LINE") cout << any_cast<_LINE*>(iterator->second) << endl;
            else cout << iterator->second.type().name() << endl;

            iterator++;
        }
    }

    cout << "shape states: ";
    if (this->shape_states.size() == 0) cout << "none" << endl;
    else
    {
        cout << endl;
        int index = 0;
        for (SPATIAL_STATE state : this->shape_states)
        {
            cout << "    " << index << ": " << this->shapes.at(index)->id() << endl;
            cout << "        position: " << state.position.x << ", " << state.position.y << endl; 
            cout << "        rotation: " << state.rotation << endl; 
            cout << "        scale: " << state.scale << endl;
            cout << "        depth: " << state.depth << endl;
            index++;
        }
    }

    cout << "cumulative shape count: " << this->total_shape_count << endl;
    cout << "cumulative child count: " << this->total_child_count << endl;
    cout << "children: ";
    if (this->children.size() == 0) cout << "none" << endl;
    else
    {
        cout << endl;
        for (_COMPONENT* child : this->children)
        {
            cout << "    " << child->address << ": " << child->id() << endl;
            cout << "        shape count: " << child->total_shape_count << endl;
            cout << "        child count: " << child->total_child_count << endl;
        }
    }

    cout << "event callbacks: ";
    if (this->component_event_callbacks.size() == 0) cout << "none" << endl;
    else
    {
        cout << endl;

        map<string, vector<function<void(event*)>>>::iterator iterator = this->component_event_callbacks.begin();
        string event_name;
        while (iterator != this->component_event_callbacks.end())
        {
            cout << "    " << iterator->first << ": " << iterator->second.size() << endl;
            iterator++;
        }
    }

    cout << endl;
}

void _COMPONENT::delete_component (bool delete_shapes)
{
    string event_name;
    for (pair reaction : this->component_state_reactors)
    {
        event_name = "react-" + this->id() + "-" + reaction.first;
        for (int i = 0; i < reaction.second.size(); i++)
        {
            get_component(reaction.second[i]).get_component_event_callbacks()->erase(event_name);
        }
        deregister_event(event_name);
    }
    deregister_component(this);
    delete_instances(this->shape_instances);
    if (delete_shapes)
    {
        for (int i = 0; i < this->shapes.size(); i++) this->shapes.at(i)->delete_shape();
    }
    if (get_components()->count(this->id()) > 0) get_components()->erase(this->id());
    delete this->address;
}
