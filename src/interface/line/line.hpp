// DEFINE
#pragma once
#define LINE _LINE


// INCLUDE
#include "../compact/compact.hpp"
#include "../component/component.hpp"


// DATA
class _LINE
{
    public:
        _LINE (string id);
        ~_LINE ();
        _COMPONENT& get_component ();
        vec2 from ();
        _LINE& from (vec2 start_point);
        vec2 to ();
        _LINE& to (vec2 end_point);
        float radius ();
        _LINE& radius (float new_radius);
        float thickness ();
        _LINE& thickness (float new_thickness);
        uint32_t red ();
        _LINE& red (uint32_t new_red);
        uint32_t green ();
        _LINE& green (uint32_t new_green);
        uint32_t blue ();
        _LINE& blue (uint32_t new_blue);
        _LINE& draw ();
        string id ();
        void print ();
        void delete_line ();

    private:
        _LINE* address = nullptr;
        _COMPONENT* line_component = nullptr;
};


// CODE
_LINE& line (string id);
