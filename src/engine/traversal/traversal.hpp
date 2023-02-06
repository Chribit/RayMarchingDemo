// INCLUDE
#include "../compact/compact.hpp"
#include "../memory/memory.hpp"
#include "../colour/colour.hpp"
#include "../bih/bih.hpp"
#include "../instance/instance.hpp"
#include "../primitive/primitive.hpp"


// CODE
colour determine_pixel_colour (glm::vec2 point);
void determine_pixel_object ();

// CAMERA
glm::vec2 transform_point_for_camera (glm::vec2 point);
