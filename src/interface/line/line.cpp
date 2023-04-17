// INCLUDE
#include "./line.hpp"
#include "../interface.hpp"


// CODE
_LINE::_LINE (string id)
{
    this->address = this;
    this->line_component = &component(id);
    this->line_component->state("line-colour-red", uint32_t(255));
    this->line_component->state("line-colour-green", uint32_t(255));
    this->line_component->state("line-colour-blue", uint32_t(255));
    this->line_component->state("line-radius", 0.0125f);
    this->line_component->state("line-thickness", 0.025f);
    this->line_component->state("line-start", vec2(0.0, 0.0));
    this->line_component->state("line-end", vec2(0.0, 0.0));
    this->line_component->add( &shape(id + "-shape").build() );
    set_line(this);
}

_LINE::~_LINE ()
{
}

_LINE& line (string id)
{
    return *(new _LINE(id));
}

_COMPONENT& _LINE::get_component ()
{
    return *(this->line_component);
}

vec2 _LINE::from ()
{
    return get_vec2(this->line_component->id(), "line-start");
}

_LINE& _LINE::from (vec2 start_point)
{
    this->line_component->state("line-start", start_point);
    return *this;
}

vec2 _LINE::to ()
{
    return get_vec2(this->line_component->id(), "line-end");
}

_LINE& _LINE::to (vec2 end_point)
{
    this->line_component->state("line-end", end_point);
    return *this;
}

float _LINE::radius ()
{
    return get_float(this->line_component->id(), "line-radius");
}

_LINE& _LINE::radius (float new_radius)
{
    this->line_component->state("line-radius", new_radius);
    return *this;
}

float _LINE::thickness ()
{
    return get_float(this->line_component->id(), "line-thickness");
}

_LINE& _LINE::thickness (float new_thickness)
{
    this->line_component->state("line-thickness", new_thickness);
    return *this;
}

uint32_t _LINE::red ()
{
    return get_uint32(this->line_component->id(), "line-colour-red");
}

_LINE& _LINE::red (uint32_t new_red)
{
    this->line_component->state("line-colour-red", new_red);
    return *this;
}

uint32_t _LINE::green ()
{
    return get_uint32(this->line_component->id(), "line-colour-green");
}

_LINE& _LINE::green (uint32_t new_green)
{
    this->line_component->state("line-colour-green", new_green);
    return *this;
}

uint32_t _LINE::blue ()
{
    return get_uint32(this->line_component->id(), "line-colour-blue");
}

_LINE& _LINE::blue (uint32_t new_blue)
{
    this->line_component->state("line-colour-blue", new_blue);
    return *this;
}

_LINE& _LINE::draw ()
{
    string id = this->line_component->id() + "-shape";

    vec2 zeroed_vector = this->to() - this->from();
    float width = length(zeroed_vector) + this->thickness();
    float angle = degrees( atan2f(zeroed_vector.y, zeroed_vector.x) );
    vec2 position = this->from() + zeroed_vector / 2.0f;

    _SHAPE old_shape = get_shape(id);
    get_shapes()->erase(id);

    this->line_component->replace(0,
        &shape(id).red( this->red() ).green( this->green() ).blue( this->blue() ).width( width ).height( this->thickness() ).rotate( angle ).x( position.x ).y( position.y ).round( this->radius() ).build()
    );
    
    old_shape.delete_shape(false);

    return *this;
}

string _LINE::id ()
{
    return this->line_component->id();
}

void _LINE::print ()
{
    this->line_component->print();
}

void _LINE::delete_line ()
{
    this->line_component->delete_component(true);
    if (get_lines()->count(this->id()) > 0) get_lines()->erase(this->id());
    delete this->address;
}
