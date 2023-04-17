// INCLUDE
#include "./event.hpp"

// DATA
static struct
{
    map<OBJECT_INSTANCE, _COMPONENT *> instance_component_map;
    vector<string> registered_events;
    _COMPONENT* currently_over;
    _COMPONENT* last_mouse_down;
    _COMPONENT* dragged_component;
    bool dragging = false;
    _COMPONENT* last_clicked;
    uint64_t last_clicked_time;
    _COMPONENT* focused;
    _COMPONENT* last_key_down;
    bool shift_pressed = false;
    bool alt_pressed = false;
    bool control_pressed = false;
    bool command_pressed = false;
    bool capslock_active = false;
    bool numlock_active = false;
    map<int, u16string> key_string_map = {
        {39, u"ä"},
        {44, u","},
        {45, u"ß"},
        {46, u"."},
        {47, u"-"},
        {48, u"0"},
        {49, u"1"},
        {50, u"2"},
        {51, u"3"},
        {52, u"4"},
        {53, u"5"},
        {54, u"6"},
        {55, u"7"},
        {56, u"8"},
        {57, u"9"},
        {59, u"ö"},
        {61, u"´"},
        {65, u"a"},
        {66, u"b"},
        {67, u"c"},
        {68, u"d"},
        {69, u"e"},
        {70, u"f"},
        {71, u"g"},
        {72, u"h"},
        {73, u"i"},
        {74, u"j"},
        {75, u"k"},
        {76, u"l"},
        {77, u"m"},
        {78, u"n"},
        {79, u"o"},
        {80, u"p"},
        {81, u"q"},
        {82, u"r"},
        {83, u"s"},
        {84, u"t"},
        {85, u"u"},
        {86, u"v"},
        {87, u"w"},
        {88, u"x"},
        {89, u"z"},
        {90, u"y"},
        {91, u"ü"},
        {92, u"#"},
        {93, u"+"},
        {96, u"<"},
        {161, u"^"},
        {39 + (1 << 8), u"Ä"},
        {44 + (1 << 8), u";"},
        {45 + (1 << 8), u"?"},
        {46 + (1 << 8), u":"},
        {47 + (1 << 8), u"_"},
        {48 + (1 << 8), u"="},
        {49 + (1 << 8), u"!"},
        {50 + (1 << 8), u"\""},
        {51 + (1 << 8), u"§"},
        {52 + (1 << 8), u"$"},
        {53 + (1 << 8), u"%"},
        {54 + (1 << 8), u"&"},
        {55 + (1 << 8), u"/"},
        {56 + (1 << 8), u"("},
        {57 + (1 << 8), u")"},
        {59 + (1 << 8), u"Ö"},
        {61 + (1 << 8), u"`"},
        {65 + (1 << 8), u"A"},
        {66 + (1 << 8), u"B"},
        {67 + (1 << 8), u"C"},
        {68 + (1 << 8), u"D"},
        {69 + (1 << 8), u"E"},
        {70 + (1 << 8), u"F"},
        {71 + (1 << 8), u"G"},
        {72 + (1 << 8), u"H"},
        {73 + (1 << 8), u"I"},
        {74 + (1 << 8), u"J"},
        {75 + (1 << 8), u"K"},
        {76 + (1 << 8), u"L"},
        {77 + (1 << 8), u"M"},
        {78 + (1 << 8), u"N"},
        {79 + (1 << 8), u"O"},
        {80 + (1 << 8), u"P"},
        {81 + (1 << 8), u"Q"},
        {82 + (1 << 8), u"R"},
        {83 + (1 << 8), u"S"},
        {84 + (1 << 8), u"T"},
        {85 + (1 << 8), u"U"},
        {86 + (1 << 8), u"V"},
        {87 + (1 << 8), u"W"},
        {88 + (1 << 8), u"X"},
        {89 + (1 << 8), u"Z"},
        {90 + (1 << 8), u"Y"},
        {91 + (1 << 8), u"Ü"},
        {92 + (1 << 8), u"'"},
        {93 + (1 << 8), u"*"},
        {96 + (1 << 8), u">"},
        {161 + (1 << 8), u"°"},
        {39 + (2 << 8), u"æ"},
        {44 + (2 << 8), u"∞"},
        {45 + (2 << 8), u"¿"},
        {46 + (2 << 8), u"…"},
        {47 + (2 << 8), u"–"},
        {48 + (2 << 8), u"≠"},
        {49 + (2 << 8), u"¡"},
        {50 + (2 << 8), u"“"},
        {51 + (2 << 8), u"¶"},
        {52 + (2 << 8), u"¢"},
        {53 + (2 << 8), u"["},
        {54 + (2 << 8), u"]"},
        {55 + (2 << 8), u"|"},
        {56 + (2 << 8), u"{"},
        {57 + (2 << 8), u"}"},
        {59 + (2 << 8), u"œ"},
        {61 + (2 << 8), u"'"},
        {65 + (2 << 8), u"å"},
        {66 + (2 << 8), u"∫"},
        {67 + (2 << 8), u"ç"},
        {68 + (2 << 8), u"∂"},
        {69 + (2 << 8), u"€"},
        {70 + (2 << 8), u"ƒ"},
        {71 + (2 << 8), u"©"},
        {72 + (2 << 8), u"ª"},
        {73 + (2 << 8), u"⁄"},
        {74 + (2 << 8), u"º"},
        {75 + (2 << 8), u"∆"},
        {76 + (2 << 8), u"@"},
        {77 + (2 << 8), u"µ"},
        {78 + (2 << 8), u"~"},
        {80 + (2 << 8), u"π"},
        {81 + (2 << 8), u"«"},
        {82 + (2 << 8), u"®"},
        {83 + (2 << 8), u"‚"},
        {84 + (2 << 8), u"†"},
        {85 + (2 << 8), u"¨"},
        {86 + (2 << 8), u"√"},
        {87 + (2 << 8), u"∑"},
        {88 + (2 << 8), u"≈"},
        {90 + (2 << 8), u"¥"},
        {91 + (2 << 8), u"•"},
        {92 + (2 << 8), u"‘"},
        {93 + (2 << 8), u"±"},
        {96 + (2 << 8), u"≤"},
        {161 + (2 << 8), u"„"},
        {39 + (3 << 8), u"Æ"},
        {44 + (3 << 8), u"˛"},
        {45 + (3 << 8), u"˙"},
        {46 + (3 << 8), u"÷"},
        {47 + (3 << 8), u"—"},
        {48 + (3 << 8), u"¯"},
        {49 + (3 << 8), u"¬"},
        {50 + (3 << 8), u"”"},
        {51 + (3 << 8), u"#"},
        {52 + (3 << 8), u"£"},
        {53 + (3 << 8), u"ﬁ"},
        {54 + (3 << 8), u"^"},
        {55 + (3 << 8), u"\\"},
        {56 + (3 << 8), u"˜"},
        {57 + (3 << 8), u"·"},
        {59 + (3 << 8), u"Œ"},
        {61 + (3 << 8), u"˚"},
        {65 + (3 << 8), u"Å"},
        {67 + (3 << 8), u"Ç"},
        {69 + (3 << 8), u"‰"},
        {71 + (3 << 8), u"Ì"},
        {72 + (3 << 8), u"Ó"},
        {74 + (3 << 8), u"ı"},
        {75 + (3 << 8), u"ˆ"},
        {76 + (3 << 8), u"ﬂ"},
        {77 + (3 << 8), u"˘"},
        {78 + (3 << 8), u"›"},
        {79 + (3 << 8), u"Ø"},
        {80 + (3 << 8), u"∏"},
        {81 + (3 << 8), u"»"},
        {82 + (3 << 8), u"¸"},
        {83 + (3 << 8), u"Í"},
        {84 + (3 << 8), u"˝"},
        {86 + (3 << 8), u"◊"},
        {87 + (3 << 8), u"„"},
        {88 + (3 << 8), u"Ù"},
        {89 + (3 << 8), u"ˇ"},
        {90 + (3 << 8), u"‡"},
        {91 + (3 << 8), u"°"},
        {92 + (3 << 8), u"’"},
        {93 + (3 << 8), u""},
        {96 + (3 << 8), u"≥"},
        {161 + (3 << 8), u"“"},
    };
    map<int, string> key_name_map = {
        {32, "space"},
        {39, "umlaut a"},
        {44, "comma"},
        {45, "eszett"},
        {46, "dot"},
        {47, "minus"},
        {48, "zero"},
        {49, "one"},
        {50, "two"},
        {51, "three"},
        {52, "four"},
        {53, "five"},
        {54, "six"},
        {55, "seven"},
        {56, "eight"},
        {57, "nine"},
        {59, "umlaut o"},
        {61, "apostrophe"},
        {65, "a"},
        {66, "b"},
        {67, "c"},
        {68, "d"},
        {69, "e"},
        {70, "f"},
        {71, "g"},
        {72, "h"},
        {73, "i"},
        {74, "j"},
        {75, "k"},
        {76, "l"},
        {77, "m"},
        {78, "n"},
        {79, "o"},
        {80, "p"},
        {81, "q"},
        {82, "r"},
        {83, "s"},
        {84, "t"},
        {85, "u"},
        {86, "v"},
        {87, "w"},
        {88, "x"},
        {89, "z"},
        {90, "y"},
        {91, "umlaut u"},
        {92, "hashtag"},
        {93, "plus"},
        {96, "lesser than"},
        {161, "circumflex accent"},
        {256, "escape"},
        {257, "enter"},
        {258, "tab"},
        {259, "backspace"},
        {262, "arrow right"},
        {263, "arrow left"},
        {264, "arrow down"},
        {265, "arrow up"},
        {280, "capslock"},
        {282, "numlock"},
        {340, "left shift"},
        {341, "left control"},
        {342, "left alt"},
        {343, "left command"},
        {344, "right shift"},
        {345, "right control"},
        {346, "right alt"},
        {347, "right command"},
    };
} event;

// CODE
void register_component (OBJECT_INSTANCE instance_pointer, _COMPONENT *component)
{
    event.instance_component_map[instance_pointer] = component;
}

_COMPONENT* lookup_component_pointer (OBJECT_INSTANCE instance_pointer)
{
    return event.instance_component_map[instance_pointer];
}

_COMPONENT& lookup_component (OBJECT_INSTANCE instance_pointer)
{
    return *(event.instance_component_map[instance_pointer]);
}

void deregister_component (_COMPONENT *component)
{
    for (int i = 0; i < component->get_shape_instances()->size(); i++)
    {
        event.instance_component_map.erase(component->get_shape_instances()->at(i));
    }
}

vector<string> get_registered_events ()
{
    return event.registered_events;
}

void register_event (string new_event)
{
    if (new_event.rfind("react-", 0) == 0) throw runtime_error("\nERROR: Event names can not start with the phrase 'react-' due to possible conflicts with component reactions.");
    if (is_event_registered(new_event)) throw runtime_error("\nERROR: The event '" + new_event + "' is already registered.");
    event.registered_events.push_back(new_event);
    sort(event.registered_events.begin(), event.registered_events.end());
}

void register_event (string new_event, bool reaction)
{
    if (is_event_registered(new_event)) throw runtime_error("\nERROR: The event '" + new_event + "' is already registered.");
    event.registered_events.push_back(new_event);
    sort(event.registered_events.begin(), event.registered_events.end());
}

bool is_event_registered (string some_event)
{
    return binary_search(event.registered_events.begin(), event.registered_events.end(), some_event);
}

void deregister_event (string some_event)
{
    vector<string>::iterator search_result = find(event.registered_events.begin(), event.registered_events.end(), some_event);
    if (search_result == event.registered_events.end()) throw runtime_error("\nERROR: The event '" + some_event + "' is not registered and can therefor not be deregistered.");
    event.registered_events.erase(search_result);
}

void set_currently_over (_COMPONENT *current_component)
{
    event.currently_over = current_component;
}

_COMPONENT* get_currently_over ()
{
    return event.currently_over;
}

void set_last_mouse_down (_COMPONENT *current_component)
{
    event.last_mouse_down = current_component;
}

_COMPONENT* get_last_mouse_down ()
{
    return event.last_mouse_down;
}

void set_dragged_component (_COMPONENT *current_component)
{
    event.dragged_component = current_component;
}

_COMPONENT* get_dragged_component ()
{
    return event.dragged_component;
}

void set_dragging (bool status)
{
    event.dragging = status;
}

bool is_dragging ()
{
    return event.dragging;
}

void set_last_clicked (_COMPONENT *current_component)
{
    event.last_clicked = current_component;
}

_COMPONENT* get_last_clicked ()
{
    return event.last_clicked;
}

void set_last_clicked_time (uint64_t time)
{
    event.last_clicked_time = time;
}

uint64_t get_last_clicked_time ()
{
    return event.last_clicked_time;
}

void set_focused (_COMPONENT *current_component)
{
    event.focused = current_component;
}

_COMPONENT* get_focused ()
{
    return event.focused;
}

void set_last_key_down (_COMPONENT *current_component)
{
    event.last_key_down = current_component;
}

_COMPONENT* get_last_key_down ()
{
    return event.last_key_down;
}

void set_modifier_keys (int key_index, int modifier_flags)
{
    event.shift_pressed = (modifier_flags & GLFW_MOD_SHIFT) > 0;
    event.control_pressed = (modifier_flags & GLFW_MOD_CONTROL) > 0;
    event.alt_pressed = (modifier_flags & GLFW_MOD_ALT) > 0;
    event.command_pressed = (modifier_flags & GLFW_MOD_SUPER) > 0;
    if (key_index == GLFW_KEY_CAPS_LOCK) event.capslock_active = !event.capslock_active;
    if (key_index == GLFW_KEY_NUM_LOCK) event.numlock_active = !event.numlock_active;
}

u16string get_key_string (int key_index)
{
    if (key_index == GLFW_KEY_LEFT_SHIFT || key_index == GLFW_KEY_RIGHT_SHIFT || 
        key_index == GLFW_KEY_LEFT_ALT || key_index == GLFW_KEY_RIGHT_ALT || 
        key_index == GLFW_KEY_LEFT_CONTROL || key_index == GLFW_KEY_RIGHT_CONTROL ||
        key_index == GLFW_KEY_LEFT_SUPER || key_index == GLFW_KEY_RIGHT_SUPER ||
        key_index == GLFW_KEY_CAPS_LOCK || key_index == GLFW_KEY_NUM_LOCK ||
        key_index == GLFW_KEY_BACKSPACE || key_index == GLFW_KEY_ENTER ||
        key_index == GLFW_KEY_LEFT || key_index == GLFW_KEY_RIGHT ||
        key_index == GLFW_KEY_UP || key_index == GLFW_KEY_DOWN ||
        key_index == GLFW_KEY_ESCAPE) return u"";
    if (key_index == GLFW_KEY_TAB) return u"  ";
    if (key_index == GLFW_KEY_SPACE) return u" ";
    if (get_shift_pressed() || get_capslock_active()) key_index += 1 << 8;
    if (get_alt_pressed()) key_index += 2 << 8;
    if (event.key_string_map.count(key_index) == 0) return u"";
    return event.key_string_map[key_index];
}

string get_key_name (int key_index)
{
    if (event.key_name_map.count(key_index) == 0) return "unknown";
    return event.key_name_map[key_index];
}

bool get_shift_pressed ()
{
    return event.shift_pressed;
}

bool get_alt_pressed ()
{
    return event.alt_pressed;
}

bool get_control_pressed ()
{
    return event.control_pressed;
}

bool get_command_pressed ()
{
    return event.command_pressed;
}

bool get_capslock_active ()
{
    return event.capslock_active;
}

bool get_numlock_active ()
{
    return event.numlock_active;
}
