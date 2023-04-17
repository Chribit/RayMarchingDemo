// INCLUDE
#include "./compact.hpp"
#include "../event/event.hpp"
#include "../component/component.hpp"
#include "../interface.hpp"


// GETTERS
bool get_bool (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "b") != 0) throw runtime_error("\nERROR: Tried retrieving type 'boolean' from event memory of other type.");
    return any_cast<bool>(event_data->get(key));
}

bool get_bool (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "b") != 0) throw runtime_error("\nERROR: Tried retrieving type 'boolean' from component state memory of other type.");
    return any_cast<bool>(get_component(component_id).state(key));
}

int get_int (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "i") != 0) throw runtime_error("\nERROR: Tried retrieving type 'int' from event memory of other type.");
    return any_cast<int>(event_data->get(key));
}

int get_int (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "i") != 0) throw runtime_error("\nERROR: Tried retrieving type 'int' from component state memory of other type.");
    return any_cast<int>(get_component(component_id).state(key));
}

float get_float (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "f") != 0) throw runtime_error("\nERROR: Tried retrieving type 'float' from event memory of other type.");
    return any_cast<float>(event_data->get(key));
}

float get_float (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "f") != 0) throw runtime_error("\nERROR: Tried retrieving type 'float' from component state memory of other type.");
    return any_cast<float>(get_component(component_id).state(key));
}

double get_double (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "d") != 0) throw runtime_error("\nERROR: Tried retrieving type 'double' from event memory of other type.");
    return any_cast<double>(event_data->get(key));
}

double get_double (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "d") != 0) throw runtime_error("\nERROR: Tried retrieving type 'double' from component state memory of other type.");
    return any_cast<double>(get_component(component_id).state(key));
}

uint32_t get_uint32 (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "j") != 0) throw runtime_error("\nERROR: Tried retrieving type 'uint32' from event memory of other type.");
    return any_cast<uint32_t>(event_data->get(key));
}

uint32_t get_uint32 (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "j") != 0) throw runtime_error("\nERROR: Tried retrieving type 'uint32' from component state memory of other type.");
    return any_cast<uint32_t>(get_component(component_id).state(key));
}

uint32_t* get_uint32p (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "Pj") != 0) throw runtime_error("\nERROR: Tried retrieving type 'uint32 pointer' from event memory of other type.");
    return any_cast<uint32_t*>(event_data->get(key));
}

uint32_t* get_uint32p (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "Pj") != 0) throw runtime_error("\nERROR: Tried retrieving type 'uint32 pointer' from component state memory of other type.");
    return any_cast<uint32_t*>(get_component(component_id).state(key));
}

string get_string (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "NSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'string' from event memory of other type.");
    return any_cast<string>(event_data->get(key));
}

string get_string (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "NSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'string' from component state memory of other type.");
    return any_cast<string>(get_component(component_id).state(key));
}

u16string get_u16string (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "NSt3__112basic_stringIDsNS_11char_traitsIDsEENS_9allocatorIDsEEEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'u16string' from event memory of other type.");
    return any_cast<u16string>(event_data->get(key));
}

u16string get_u16string (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "NSt3__112basic_stringIDsNS_11char_traitsIDsEENS_9allocatorIDsEEEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'u16string' from component state memory of other type.");
    return any_cast<u16string>(get_component(component_id).state(key));
}

vec2 get_vec2 (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "N3glm3vecILi2EfLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'vec2' from event memory of other type.");
    return any_cast<vec2>(event_data->get(key));
}

vec2 get_vec2 (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "N3glm3vecILi2EfLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'vec2' from component state memory of other type.");
    return any_cast<vec2>(get_component(component_id).state(key));
}

uvec2 get_uvec2 (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "N3glm3vecILi2EjLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'uvec2' from event memory of other type.");
    return any_cast<uvec2>(event_data->get(key));
}

uvec2 get_uvec2 (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "N3glm3vecILi2EjLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'uvec2' from component state memory of other type.");
    return any_cast<uvec2>(get_component(component_id).state(key));
}

dvec2 get_dvec2 (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "N3glm3vecILi2EdLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'dvec2' from event memory of other type.");
    return any_cast<dvec2>(event_data->get(key));
}

dvec2 get_dvec2 (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "N3glm3vecILi2EdLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'dvec2' from component state memory of other type.");
    return any_cast<dvec2>(get_component(component_id).state(key));
}

bvec2 get_bvec2 (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "N3glm3vecILi2EbLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'bvec2' from event memory of other type.");
    return any_cast<bvec2>(event_data->get(key));
}

bvec2 get_bvec2 (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "N3glm3vecILi2EbLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'bvec2' from component state memory of other type.");
    return any_cast<bvec2>(get_component(component_id).state(key));
}

vec3 get_vec3 (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "N3glm3vecILi3EfLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'vec3' from event memory of other type.");
    return any_cast<vec3>(event_data->get(key));
}

vec3 get_vec3 (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "N3glm3vecILi3EfLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'vec3' from component state memory of other type.");
    return any_cast<vec3>(get_component(component_id).state(key));
}

uvec3 get_uvec3 (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "N3glm3vecILi3EjLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'uvec3' from event memory of other type.");
    return any_cast<uvec3>(event_data->get(key));
}

uvec3 get_uvec3 (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "N3glm3vecILi3EjLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'uvec3' from component state memory of other type.");
    return any_cast<uvec3>(get_component(component_id).state(key));
}

dvec3 get_dvec3 (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "N3glm3vecILi3EdLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'dvec3' from event memory of other type.");
    return any_cast<dvec3>(event_data->get(key));
}

dvec3 get_dvec3 (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "N3glm3vecILi3EdLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'dvec3' from component state memory of other type.");
    return any_cast<dvec3>(get_component(component_id).state(key));
}

bvec3 get_bvec3 (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "N3glm3vecILi3EbLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'bvec3' from event memory of other type.");
    return any_cast<bvec3>(event_data->get(key));
}

bvec3 get_bvec3 (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "N3glm3vecILi3EbLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'bvec3' from component state memory of other type.");
    return any_cast<bvec3>(get_component(component_id).state(key));
}

vec4 get_vec4 (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "N3glm3vecILi4EfLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'vec4' from event memory of other type.");
    return any_cast<vec4>(event_data->get(key));
}

vec4 get_vec4 (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "N3glm3vecILi4EfLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'vec4' from component state memory of other type.");
    return any_cast<vec4>(get_component(component_id).state(key));
}

uvec4 get_uvec4 (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "N3glm3vecILi4EjLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'uvec4' from event memory of other type.");
    return any_cast<uvec4>(event_data->get(key));
}

uvec4 get_uvec4 (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "N3glm3vecILi4EjLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'uvec4' from component state memory of other type.");
    return any_cast<uvec4>(get_component(component_id).state(key));
}

dvec4 get_dvec4 (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "N3glm3vecILi4EdLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'dvec4' from event memory of other type.");
    return any_cast<dvec4>(event_data->get(key));
}

dvec4 get_dvec4 (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "N3glm3vecILi4EdLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'dvec4' from component state memory of other type.");
    return any_cast<dvec4>(get_component(component_id).state(key));
}

bvec4 get_bvec4 (event* event_data, string key)
{
    if (strcmp(event_data->get(key).type().name(), "N3glm3vecILi4EbLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'bvec4' from event memory of other type.");
    return any_cast<bvec4>(event_data->get(key));
}

bvec4 get_bvec4 (string component_id, string key)
{
    if (strcmp(get_component(component_id).state(key).type().name(), "N3glm3vecILi4EbLNS_9qualifierE0EEE") != 0) throw runtime_error("\nERROR: Tried retrieving type 'bvec4' from component state memory of other type.");
    return any_cast<bvec4>(get_component(component_id).state(key));
}
