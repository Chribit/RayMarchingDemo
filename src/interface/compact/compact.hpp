// DEFINE
#pragma once


// INCLUDE
#include "cstring"
#include "iostream"
#include "vector"
#include "algorithm"
#include "functional"
#include "numeric"
#include "glm/glm.hpp"
#include "map"
#include "any"
#include "codecvt"


// GRAPHICS INCLUDE
#ifndef DEFINED_G
#define DEFINED_G
#include "../../../../Graphics/release/0.2.7/graphics.hpp"
#endif


// NAMESPACE
using namespace std;
using glm::vec2;
using glm::uvec2;
using glm::bvec2;
using glm::dvec2;
using glm::vec3;
using glm::uvec3;
using glm::bvec3;
using glm::dvec3;
using glm::vec4;
using glm::uvec4;
using glm::bvec4;
using glm::dvec4;
using glm::mat2x2;
using glm::degrees;
using glm::radians;
using glm::sign;
using glm::length;


// PRE
class event;


// GETTERS
bool get_bool (event* event_data, string key);
bool get_bool (string component_id, string key);
int get_int (event* event_data, string key);
int get_int (string component_id, string key);
float get_float (event* event_data, string key);
float get_float (string component_id, string key);
double get_double (event* event_data, string key);
double get_double (string component_id, string key);
uint32_t get_uint32 (event* event_data, string key);
uint32_t get_uint32 (string component_id, string key);
uint32_t* get_uint32p (event* event_data, string key);
uint32_t* get_uint32p (string component_id, string key);
string get_string (event* event_data, string key);
string get_string (string component_id, string key);
u16string get_u16string (event* event_data, string key);
u16string get_u16string (string component_id, string key);
vec2 get_vec2 (event* event_data, string key);
vec2 get_vec2 (string component_id, string key);
uvec2 get_uvec2 (event* event_data, string key);
uvec2 get_uvec2 (string component_id, string key);
dvec2 get_dvec2 (event* event_data, string key);
dvec2 get_dvec2 (string component_id, string key);
bvec2 get_bvec2 (event* event_data, string key);
bvec2 get_bvec2 (string component_id, string key);
vec3 get_vec3 (event* event_data, string key);
vec3 get_vec3 (string component_id, string key);
uvec3 get_uvec3 (event* event_data, string key);
uvec3 get_uvec3 (string component_id, string key);
dvec3 get_dvec3 (event* event_data, string key);
dvec3 get_dvec3 (string component_id, string key);
bvec3 get_bvec3 (event* event_data, string key);
bvec3 get_bvec3 (string component_id, string key);
vec4 get_vec4 (event* event_data, string key);
vec4 get_vec4 (string component_id, string key);
uvec4 get_uvec4 (event* event_data, string key);
uvec4 get_uvec4 (string component_id, string key);
dvec4 get_dvec4 (event* event_data, string key);
dvec4 get_dvec4 (string component_id, string key);
bvec4 get_bvec4 (event* event_data, string key);
bvec4 get_bvec4 (string component_id, string key);