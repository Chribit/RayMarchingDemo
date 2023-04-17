// INCLUDE
#include "../compact/compact.hpp"


// CODE
void on_initialisation (function<void()> callback);
void run_initialisation_callbacks ();
void on_render_completion (function<void()> callback);
void run_render_completion_callbacks ();


// MOUSE
void on_mouse_move (function<void(glm::vec2)> callback);
void run_mouse_move_callbacks ();
void on_mouse_down (function<void(glm::vec2)> callback);
void run_mouse_down_callbacks ();
void on_mouse_up (function<void(glm::vec2)> callback);
void run_mouse_up_callbacks ();
void on_mouse_scroll (function<void(glm::vec2, glm::dvec2)> callback);
void run_mouse_scroll_callbacks (glm::dvec2 mouse_offsets);


// WINDOW
void on_mouse_enter (function<void(glm::vec2)> callback);
void run_mouse_enter_callbacks ();
void on_mouse_leave (function<void(glm::vec2)> callback);
void run_mouse_leave_callbacks ();
void on_window_resize (function<void(glm::uvec2)> callback);
void run_window_resize_callbacks ();


// KEYBOARD
void on_key_down (function<void(int, int, int)> callback);
void run_key_down_callbacks (int key, int scancode, int mods);
void on_key_up (function<void(int, int, int)> callback);
void run_key_up_callbacks (int key, int scancode, int mods);


// CLIPBOARD
const char* get_clipboard ();
void set_clipboard (const char* string);


// ACCESS
vector<function<void()>>* get_initialisation_callbacks ();
void set_initialisation_callbacks (vector<function<void()>> callbacks);
vector<function<void()>>* get_render_completion_callbacks ();
void set_render_completion_callbacks (vector<function<void()>> callbacks);
vector<function<void(glm::vec2)>>* get_mouse_move_callbacks ();
void set_mouse_move_callbacks (vector<function<void(glm::vec2)>> callbacks);
vector<function<void(glm::vec2)>>* get_mouse_down_callbacks ();
void set_mouse_down_callbacks (vector<function<void(glm::vec2)>> callbacks);
vector<function<void(glm::vec2)>>* get_mouse_up_callbacks ();
void set_mouse_up_callbacks (vector<function<void(glm::vec2)>> callbacks);
vector<function<void(glm::vec2, glm::dvec2)>>* get_mouse_scroll_callbacks ();
void set_mouse_scroll_callbacks (vector<function<void(glm::vec2, glm::dvec2)>> callbacks);
vector<function<void(glm::vec2)>>* get_mouse_enter_callbacks ();
void set_mouse_enter_callbacks (vector<function<void(glm::vec2)>> callbacks);
vector<function<void(glm::vec2)>>* get_mouse_leave_callbacks ();
void set_mouse_leave_callbacks (vector<function<void(glm::vec2)>> callbacks);
vector<function<void(glm::uvec2)>>* get_window_resize_callbacks ();
void set_window_resize_callbacks (vector<function<void(glm::uvec2)>> callbacks);
vector<function<void(int, int, int)>>* get_key_down_callbacks ();
void set_key_down_callbacks (vector<function<void(int, int, int)>> callbacks);
vector<function<void(int, int, int)>>* get_key_up_callbacks ();
void set_key_up_callbacks (vector<function<void(int, int, int)>> callbacks);
