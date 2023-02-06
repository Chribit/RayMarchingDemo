// INCLUDE
#include "./colour.hpp"


// CODE
uint32_t encode_colour (colour colour_channels)
{
    uint32_t colour = colour_channels.red;
    colour = colour << 8 | colour_channels.green;
    colour = colour << 8 | colour_channels.blue;
    colour = colour << 8 | colour_channels.alpha;
    return colour;
}

colour decode_colour (uint32_t encoded_colour)
{
    colour output;
    output.red = (encoded_colour & 0xFF000000) >> 24;
    output.green = (encoded_colour & 0x00FF0000) >> 16;
    output.blue = (encoded_colour & 0x0000FF00) >> 8;
    output.alpha = (encoded_colour & 0x000000FF) >> 0;
    return output;
}
