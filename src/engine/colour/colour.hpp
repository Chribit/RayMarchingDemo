// INCLUDE
#include "../compact/compact.hpp"


// DATA
#ifndef DEFINED_COL
#define DEFINED_COL
struct colour {
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint32_t alpha = 255;
};
#endif


// CODE
uint32_t encode_colour (colour colour_channels);
colour decode_colour (uint32_t encoded_colour);
