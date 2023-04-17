// INCLUDE
#include "./event.hpp"

// DATA
static struct {
    vector<function<void()>> initialisation;
    vector<function<void()>> render_completion;
    vector<function<void(glm::vec2)>> mouse_move;
    vector<function<void(glm::vec2)>> mouse_down;
    vector<function<void(glm::vec2)>> mouse_up;
    vector<function<void(glm::vec2, glm::dvec2)>> mouse_scroll;
    vector<function<void(glm::vec2)>> mouse_enter;
    vector<function<void(glm::vec2)>> mouse_leave;
    vector<function<void(glm::uvec2)>> window_resize;
    vector<function<void(int, int, int)>> key_down;
    vector<function<void(int, int, int)>> key_up;
} event;

// CODE
vector<function<void()>>* get_initialisation_callbacks ()
{
    return &event.initialisation;
}

void set_initialisation_callbacks (vector<function<void()>> callbacks)
{
    event.initialisation = callbacks;
}

vector<function<void()>>* get_render_completion_callbacks ()
{
    return &event.render_completion;
}

void set_render_completion_callbacks (vector<function<void()>> callbacks)
{
    event.render_completion = callbacks;
}

vector<function<void(glm::vec2)>>* get_mouse_move_callbacks ()
{
    return &event.mouse_move;
}

void set_mouse_move_callbacks (vector<function<void(glm::vec2)>> callbacks)
{
    event.mouse_move = callbacks;
}

vector<function<void(glm::vec2)>>* get_mouse_down_callbacks ()
{
    return &event.mouse_down;
}

void set_mouse_down_callbacks (vector<function<void(glm::vec2)>> callbacks)
{
    event.mouse_down = callbacks;
}

vector<function<void(glm::vec2)>>* get_mouse_up_callbacks ()
{
    return &event.mouse_up;
}

void set_mouse_up_callbacks (vector<function<void(glm::vec2)>> callbacks)
{
    event.mouse_up = callbacks;
}

vector<function<void(glm::vec2, glm::dvec2)>>* get_mouse_scroll_callbacks ()
{
    return &event.mouse_scroll;
}

void set_mouse_scroll_callbacks (vector<function<void(glm::vec2, glm::dvec2)>> callbacks)
{
    event.mouse_scroll = callbacks;
}

vector<function<void(glm::vec2)>>* get_mouse_enter_callbacks ()
{
    return &event.mouse_enter;
}

void set_mouse_enter_callbacks (vector<function<void(glm::vec2)>> callbacks)
{
    event.mouse_enter = callbacks;
}

vector<function<void(glm::vec2)>>* get_mouse_leave_callbacks ()
{
    return &event.mouse_leave;
}

void set_mouse_leave_callbacks (vector<function<void(glm::vec2)>> callbacks)
{
    event.mouse_leave = callbacks;
}

vector<function<void(glm::uvec2)>>* get_window_resize_callbacks ()
{
    return &event.window_resize;
}

void set_window_resize_callbacks (vector<function<void(glm::uvec2)>> callbacks)
{
    event.window_resize = callbacks;
}

vector<function<void(int, int, int)>>* get_key_down_callbacks ()
{
    return &event.key_down;
}

void set_key_down_callbacks (vector<function<void(int, int, int)>> callbacks)
{
    event.key_down = callbacks;
}

vector<function<void(int, int, int)>>* get_key_up_callbacks ()
{
    return &event.key_up;
}

void set_key_up_callbacks (vector<function<void(int, int, int)>> callbacks)
{
    event.key_up = callbacks;
}

