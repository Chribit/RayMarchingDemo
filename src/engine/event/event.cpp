// INCLUDE
#include "./event.hpp"
#include "../engine.hpp"


// ENGINE
void on_initialisation (function<void()> callback)
{
    get_initialisation_callbacks()->push_back(callback);
}

void run_initialisation_callbacks ()
{
    for (int i = 0; i < get_initialisation_callbacks()->size(); i++)
    {
        get_initialisation_callbacks()->at(i)();
    }
}

void on_render_completion (function<void()> callback)
{
    get_render_completion_callbacks()->push_back(callback);
}

void run_render_completion_callbacks ()
{
    for (int i = 0; i < get_render_completion_callbacks()->size(); i++)
    {
        get_render_completion_callbacks()->at(i)();
    }
}


// MOUSE
void on_mouse_move (function<void(glm::vec2)> callback)
{
    get_mouse_move_callbacks()->push_back(callback);
}

void run_mouse_move_callbacks ()
{
    glm::vec2 mouse_coordinates = get_mouse_coordinates();
    for (int i = 0; i < get_mouse_move_callbacks()->size(); i++)
    {
        get_mouse_move_callbacks()->at(i)( mouse_coordinates );
    }
}

void on_mouse_down (function<void(glm::vec2)> callback)
{
    get_mouse_down_callbacks()->push_back(callback);
}

void run_mouse_down_callbacks ()
{
    glm::vec2 mouse_coordinates = get_mouse_coordinates();
    for (int i = 0; i < get_mouse_down_callbacks()->size(); i++)
    {
        get_mouse_down_callbacks()->at(i)( mouse_coordinates );
    }
}

void on_mouse_up (function<void(glm::vec2)> callback)
{
    get_mouse_up_callbacks()->push_back(callback);
}

void run_mouse_up_callbacks ()
{
    glm::vec2 mouse_coordinates = get_mouse_coordinates();
    for (int i = 0; i < get_mouse_up_callbacks()->size(); i++)
    {
        get_mouse_up_callbacks()->at(i)( mouse_coordinates );
    }
}

void on_mouse_scroll (function<void(glm::vec2, glm::dvec2)> callback)
{
    get_mouse_scroll_callbacks()->push_back(callback);
}

void run_mouse_scroll_callbacks (glm::dvec2 mouse_offsets)
{
    glm::vec2 mouse_coordinates = get_mouse_coordinates();
    for (int i = 0; i < get_mouse_scroll_callbacks()->size(); i++)
    {
        get_mouse_scroll_callbacks()->at(i)( mouse_coordinates, mouse_offsets );
    }
}


// WINDOW
void on_mouse_enter (function<void(glm::vec2)> callback)
{
    get_mouse_enter_callbacks()->push_back(callback);
}

void run_mouse_enter_callbacks ()
{
    for (int i = 0; i < get_mouse_enter_callbacks()->size(); i++)
    {
        get_mouse_enter_callbacks()->at(i)( get_mouse_coordinates() );
    }
}

void on_mouse_leave (function<void(glm::vec2)> callback)
{
    get_mouse_leave_callbacks()->push_back(callback);
}

void run_mouse_leave_callbacks ()
{
    for (int i = 0; i < get_mouse_leave_callbacks()->size(); i++)
    {
        get_mouse_leave_callbacks()->at(i)( get_mouse_coordinates() );
    }
}

void on_window_resize (function<void(glm::uvec2)> callback)
{
    get_window_resize_callbacks()->push_back(callback);
}

void run_window_resize_callbacks ()
{
    glm::uvec2 window_size = get_push_constant_window_size();
    for (int i = 0; i < get_window_resize_callbacks()->size(); i++)
    {
        get_window_resize_callbacks()->at(i)( window_size );
    }
}


// KEYBOARD
void on_key_down (function<void(int)> callback)
{
    get_key_down_callbacks()->push_back(callback);
}

void run_key_down_callbacks (int key)
{
    for (int i = 0; i < get_key_down_callbacks()->size(); i++)
    {
        get_key_down_callbacks()->at(i)( key );
    }
}

void on_key_up (function<void(int)> callback)
{
    get_key_up_callbacks()->push_back(callback);
}

void run_key_up_callbacks (int key)
{
    for (int i = 0; i < get_key_up_callbacks()->size(); i++)
    {
        get_key_up_callbacks()->at(i)( key );
    }
}


// CLIPBOARD
const char* get_clipboard ()
{
    return glfwGetClipboardString( get_window() );
}

void set_clipboard (const char* string)
{
    glfwSetClipboardString( get_window(), string );
}
