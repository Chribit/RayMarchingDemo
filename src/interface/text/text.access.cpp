// INCLUDE
#include "./text.hpp"


// DATA
static struct {
    map<u16string, _SHAPE*> characters;
} text_data;


// CODE
void set_character (u16string id, _SHAPE* character)
{
    text_data.characters[id] = character;
}

bool has_character (u16string id)
{
    return text_data.characters.count(id) > 0;
}

_SHAPE* get_character (u16string character, colour font_colour)
{
    u16string id = character + to_u16string(font_colour.red) + to_u16string(font_colour.green) + to_u16string(font_colour.blue);
    return text_data.characters.count(id) == 1 ? text_data.characters.at(id) : text_data.characters.at(u"NULL" + to_u16string(font_colour.red) + to_u16string(font_colour.green) + to_u16string(font_colour.blue));
}
