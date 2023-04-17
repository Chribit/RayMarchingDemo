// INCLUDE
#include "./compact/compact.hpp"
#include "./shape/shape.hpp"
#include "./component/component.hpp"
#include "./line/line.hpp"
#include "./event/event.hpp"
#include "./text/text.hpp"


// CODE
void create_interface (string application_name, uvec2 window_size, function<void()> interface_code);
void cleanup_interface ();


// ACCESS
_COMPONENT& get_root ();
_COMPONENT* get_root_pointer ();
map<string, _SHAPE*>* get_shapes ();
_SHAPE& get_shape (string id);
void set_shape (_SHAPE* new_shape);
map<string, _COMPONENT*>* get_components ();
_COMPONENT& get_component (string id);
void set_component (_COMPONENT* new_component);
map<string, _LINE*>* get_lines ();
_LINE& get_line (string id);
void set_line (_LINE* new_line);
uint32_t get_instance_count ();
void increase_instance_count ();
