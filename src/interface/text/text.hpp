// DEFINE
#pragma once


// INCLUDE
#include "../compact/compact.hpp"
#include "../shape/shape.hpp"
#include "../component/component.hpp"


// DATA
enum alignment
{
    LEFT,
    CENTER,
    RIGHT
};


// CODE
u16string to_u16string(int number);
void initialise_font (colour font_colour);
_COMPONENT& text (string new_component_id, u16string characters, colour font_colour);
void replace_text (string component_id, u16string new_text, colour new_colour);


// ACCESS
void set_character (u16string id, _SHAPE* character);
bool has_character (u16string id);
_SHAPE* get_character (u16string character, colour font_colour);
