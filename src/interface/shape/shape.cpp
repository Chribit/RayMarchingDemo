// INCLUDE
#include "./shape.hpp"
#include "../interface.hpp"


// CODE
_SHAPE::_SHAPE (string id)
{
    if (get_shapes()->count(id) != 0) throw runtime_error("\nERROR: Shape id '" + id + "' already exists.");
    this->address = this;
    this->shape_id = id;
    set_shape(this);
}

_SHAPE::~_SHAPE ()
{
}

_SHAPE& shape (string id)
{
    return *(new _SHAPE(id));
}

_SHAPE& _SHAPE::x (float new_x)
{
    if (this->is_built) return *this;
    this->position_data.x = new_x;
    return *this;
}

_SHAPE& _SHAPE::y (float new_y)
{
    if (this->is_built) return *this;
    this->position_data.y = new_y;
    return *this;
}

_SHAPE& _SHAPE::width (float new_width)
{
    if (this->is_built) return *this;
    this->dimension_data.x = new_width / 2.0;
    return *this;
}

_SHAPE& _SHAPE::height (float new_height)
{
    if (this->is_built) return *this;
    this->dimension_data.y = new_height / 2.0;
    return *this;
}

_SHAPE& _SHAPE::radius (float new_radius)
{
    if (this->is_built) return *this;
    this->radius_data = new_radius;
    return *this;
}

_SHAPE& _SHAPE::round (float new_radius)
{
    if (this->is_built) return *this;
    this->radius_data = new_radius;
    this->dimension_data.x = glm::max(0.0f, this->dimension_data.x - new_radius);
    this->dimension_data.y = glm::max(0.0f, this->dimension_data.y - new_radius);
    return *this;
}

_SHAPE& _SHAPE::thickness (float new_thickness)
{
    if (this->is_built) return *this;
    this->thickness_data = new_thickness;
    return *this;
}

_SHAPE& _SHAPE::rotate (float new_rotation)
{
    if (this->is_built) return *this;
    this->rotation_data = new_rotation;
    return *this;
}

_SHAPE& _SHAPE::scale (float new_scale)
{
    if (this->is_built) return *this;
    this->radius_data *= new_scale;
    this->dimension_data *= new_scale;
    this->thickness_data *= new_scale;
    return *this;
}

_SHAPE& _SHAPE::add (_SHAPE* joined_shape)
{
    if (this->is_built) return *this;
    this->joined_shapes.push_back(joined_shape);
    return *this;
}

_SHAPE& _SHAPE::repeat (uint16_t x_repetition, uint16_t y_repetition)
{
    if (this->is_built) return *this;
    this->x_repetition = x_repetition;
    this->y_repetition = y_repetition;
    return *this;
}

_SHAPE& _SHAPE::subtract (_SHAPE* operator_shape)
{
    if (this->is_built) return *this;
    this->subtraction_operators.push_back(operator_shape);
    return *this;
}

_SHAPE& _SHAPE::intersect (_SHAPE* operator_shape)
{
    if (this->is_built) return *this;
    this->intersection_operators.push_back(operator_shape);
    return *this;
}

_SHAPE& _SHAPE::red (uint32_t new_red)
{
    if (this->is_built) return *this;
    this->colour_data.red = glm::min(uint32_t(255), new_red);
    return *this;
}

_SHAPE& _SHAPE::green (uint32_t new_green)
{
    if (this->is_built) return *this;
    this->colour_data.green = glm::min(uint32_t(255), new_green);
    return *this;
}

_SHAPE& _SHAPE::blue (uint32_t new_blue)
{
    if (this->is_built) return *this;
    this->colour_data.blue = glm::min(uint32_t(255), new_blue);
    return *this;
}

_SHAPE& _SHAPE::visible (bool is_visible)
{
    if (this->is_built) return *this;
    this->colour_data.alpha = is_visible ? 255 : 0;
    return *this;
}

SHAPE_DATA _SHAPE::data ()
{
    SHAPE_DATA data;
    data.position_data = this->position_data;
    data.dimension_data = this->dimension_data;
    data.radius_data = this->radius_data;
    data.thickness_data = this->thickness_data;
    data.rotation_data = this->rotation_data;
    data.x_repetition = this->x_repetition;
    data.y_repetition = this->y_repetition;
    data.colour_data = this->colour_data;
    return data;
}

vector<_SHAPE*>* _SHAPE::additions ()
{
    return &this->joined_shapes;
}

vector<_SHAPE*>* _SHAPE::subtractions ()
{
    return &this->subtraction_operators;
}

vector<_SHAPE*>* _SHAPE::intersections ()
{
    return &this->intersection_operators;
}

int build_shape_primitive (_SHAPE* target_shape, operation operation_type)
{
    SHAPE_DATA data = target_shape->data();
    return create_primitive({
        .position = data.position_data,
        .dimensions = data.dimension_data,
        .radius = data.radius_data,
        .thickness = data.thickness_data
    }, {
        .rotation = data.rotation_data,
        .type = operation_type,
        .x_repeat = data.x_repetition,
        .y_repeat = data.y_repetition,
        .target = 0
    }, {
        .red = data.colour_data.red,
        .green = data.colour_data.green,
        .blue = data.colour_data.blue,
        .alpha = data.colour_data.alpha
    });
}

vector<int> build_shape (_SHAPE* target_shape)
{
    vector<int> hierarchy = {};
    _SHAPE* current_shape;

    hierarchy.push_back( build_shape_primitive(target_shape, REPETITION) );

    vector<_SHAPE*>* subtractions = target_shape->subtractions();
    for (int i = 0; i < subtractions->size(); i++)
    {
        current_shape = subtractions->at(i);
        hierarchy.push_back( build_shape_primitive(current_shape, DIFFERENCE) );
    }

    vector<_SHAPE*>* intersections = target_shape->intersections();
    for (int i = 0; i < intersections->size(); i++)
    {
        current_shape = intersections->at(i);
        hierarchy.push_back( build_shape_primitive(current_shape, INTERSECTION) );
    }

    return hierarchy;
}

_SHAPE& _SHAPE::build ()
{
    if (this->is_built) throw runtime_error("\nERROR: This shape has already been built.");

    vector<vector<int>> primitive_hierarchies;
    vector<_SHAPE*> shape_queue = { this };
    vector<_SHAPE*>* additional_shapes;

    int current_shape_index = 0;
    _SHAPE* current_shape = shape_queue.at( current_shape_index );

    while (true)
    {
        current_shape = shape_queue.at( current_shape_index );

        primitive_hierarchies.push_back( build_shape(current_shape) );

        current_shape_index++;

        additional_shapes = current_shape->additions();
        shape_queue.insert(
            shape_queue.begin() + current_shape_index,
            additional_shapes->begin(),
            additional_shapes->end()
        );

        if (current_shape_index >= shape_queue.size()) break;
    }

    this->shape_object = create_object( primitive_hierarchies );

    this->is_built = true;

    return *this;
}

bool _SHAPE::built ()
{
    return this->is_built;
}

OBJECT _SHAPE::get_build ()
{
    if (!this->built()) throw runtime_error("\nERROR: The shape has not been built while attempting to retrieve the build.");
    return this->shape_object;
}

string _SHAPE::id ()
{
    return this->shape_id;
}

void _SHAPE::print ()
{
    cout << endl << this->address << "\nposition: " << this->position_data.x << ", " << this->position_data.y << "\nradius: " << this->radius_data << "\ndimensions: " << this->dimension_data.x * 2.0 << ", " << this->dimension_data.y * 2.0 << "\nthickness: " << this->thickness_data << "\nrotation: " << this->rotation_data;

    if (this->x_repetition != 0 || this->y_repetition != 0) cout << "\nrepetition: " << this->x_repetition << ", " << this->y_repetition;

    cout << "\ncolour: " << this->colour_data.red << ", " << this->colour_data.green << ", " << this->colour_data.blue << ", " << this->colour_data.alpha << endl << endl;
}

void _SHAPE::delete_shape ()
{
    if (this->is_built) delete_object(this->shape_object);
    get_shapes()->erase(this->id());
    delete this->address;
}
