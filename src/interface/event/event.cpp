// INCLUDE
#include "./event.hpp"
#include "../interface.hpp"


// CODE
void initialise_events ()
{
    // OPTIMISE: capslock is always initialised as false and GLFW can't recognise the initial capslock state

    set_currently_over( get_root_pointer() );
    set_last_mouse_down( nullptr );
    set_last_clicked( nullptr );
    set_focused( get_root_pointer() );
    set_last_clicked( nullptr );
    set_last_clicked_time( 0 );

    register_event("click");
    register_event("double click");
    register_event("drag");
    register_event("drag start");
    register_event("drop");
    register_event("hot key down");
    register_event("hot key press");
    register_event("hot key up");
    register_event("hover");
    register_event("key down");
    register_event("key press");
    register_event("key up");
    register_event("mouse down");
    register_event("mouse enter");
    register_event("mouse leave");
    register_event("mouse move");
    register_event("mouse scroll");
    register_event("mouse up");
    register_event("window resize");

    on_render_completion([]() {
        if ( get_currently_over() != lookup_component_pointer( get_front_object() ))
        {
            broadcast_event(
                *get_currently_over(), "mouse leave", {
                    {"component id", get_currently_over()->id()},
                    {"mouse position", get_mouse_coordinates()},
                    {"has hovering", get_currently_over()->get_hovering()}
                }
            );
            broadcast_event(
                lookup_component( get_front_object() ), "mouse enter", {
                    {"component id", lookup_component( get_front_object() ).id()},
                    {"mouse position", get_mouse_coordinates()},
                    {"has hovering", get_currently_over()->get_hovering()}
                }
            );
            set_currently_over( lookup_component_pointer( get_front_object() ));
        }

        broadcast_event(
            *get_currently_over(), "hover", {
                {"component id", get_currently_over()->id()},
                {"mouse position", get_mouse_coordinates()},
                {"has hovering", get_currently_over()->get_hovering()}
            }
        );
    });

    on_window_resize([](uvec2 window_dimensions) {
        broadcast_event(
            get_root(), "window resize", {
                {"window width", window_dimensions.x},
                {"window height", window_dimensions.y},
                {"window dimensions", window_dimensions}
            }
        );
    });

    on_mouse_down([](vec2 mouse_position) {
        broadcast_event(
            lookup_component( get_front_object() ), "mouse down", {
                {"mouse position", mouse_position}
            }
        );
        set_last_mouse_down( lookup_component_pointer(get_front_object()) );
    });
    on_mouse_up([](vec2 mouse_position) {
        broadcast_event(
            lookup_component( get_front_object() ), "mouse up", {
                {"mouse position", mouse_position}
            }
        );

        if (lookup_component_pointer(get_front_object()) == get_last_mouse_down())
        {
            bool double_click_fired = false;

            broadcast_event(
                *get_last_mouse_down(), "click", {
                    {"mouse position", mouse_position}
                }
            );

            set_focused( lookup_component_pointer(get_front_object()) );

            if (lookup_component_pointer(get_front_object()) == get_last_clicked() &&
                ((uint64_t) duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) - get_last_clicked_time() < 500)
            {
                broadcast_event(
                    *get_last_clicked(), "double click", {
                        {"mouse position", mouse_position}
                    }
                );
                double_click_fired = true;
            }

            set_last_clicked( double_click_fired ? nullptr : lookup_component_pointer(get_front_object()) );
            set_last_clicked_time( double_click_fired ? 0 : (uint64_t) duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
        }

        if (is_dragging())
        {
            broadcast_event(
                *get_dragged_component(), "drop", {
                    {"mouse position", mouse_position}
                }
            );

            set_focused( get_dragged_component() );

            set_dragging(false);
            set_dragged_component(nullptr);
        }

        set_last_mouse_down( nullptr );
    });

    on_mouse_enter([](vec2 mouse_position) { broadcast_event(
        get_root(), "mouse enter", {
            {"component id", get_root().id()},
            {"mouse position", mouse_position},
            {"has hovering", get_root().get_hovering()}
    }); });
    on_mouse_leave([](vec2 mouse_position) { broadcast_event(
        get_root(), "mouse leave", {
            {"component id", get_root().id()},
            {"mouse position", mouse_position},
            {"has hovering", get_root().get_hovering()}
    }); });

    on_mouse_move([](vec2 mouse_position) {
        broadcast_event(
            get_root(), "mouse move", {
                {"mouse position", mouse_position}
            }
        );
        broadcast_event(
            *get_focused(), "mouse move", {
                {"mouse position", mouse_position}
            }
        );
        if (get_last_mouse_down() == lookup_component_pointer(get_front_object()))
        {
            set_dragged_component( lookup_component_pointer( get_front_object() ));
            broadcast_event(
                *get_dragged_component(), "drag start", {
                    {"mouse position", mouse_position}
                }
            );
            set_dragging(true);
            set_last_mouse_down( nullptr );
        }
        if (is_dragging())
        {
            broadcast_event(
                *get_dragged_component(), "drag", {
                    {"mouse position", mouse_position}
                }
            );
        }
    });
    on_mouse_scroll([](vec2 mouse_position, dvec2 scroll_direction) { broadcast_event(
        *get_focused(), "mouse scroll", {
            {"mouse position", mouse_position}, 
            {"scroll direction", scroll_direction}
    }); });

    on_key_down([](int key, int scancode, int mods) {
        set_modifier_keys(key, mods);
        broadcast_event(
            *get_focused(), "key down", {
                {"string", get_key_string(key)},
                {"name", get_key_name(key)},
                {"shift pressed", get_shift_pressed()},
                {"alt pressed", get_alt_pressed()},
                {"control pressed", get_control_pressed()},
                {"command pressed", get_command_pressed()},
                {"capslock active", get_capslock_active()},
                {"numlock active", get_numlock_active()}
            }
        );
        set_last_key_down( get_focused() );
        broadcast_event(
            get_root(), "hot key down", {
                {"string", get_key_string(key)},
                {"name", get_key_name(key)},
                {"shift pressed", get_shift_pressed()},
                {"alt pressed", get_alt_pressed()},
                {"control pressed", get_control_pressed()},
                {"command pressed", get_command_pressed()},
                {"capslock active", get_capslock_active()},
                {"numlock active", get_numlock_active()}
            }
        );
    });
    on_key_up([](int key, int scancode, int mods) {
        set_modifier_keys(key, mods);
        broadcast_event(
            *get_focused(), "key up", {
                {"string", get_key_string(key)},
                {"name", get_key_name(key)},
                {"shift pressed", get_shift_pressed()},
                {"alt pressed", get_alt_pressed()},
                {"control pressed", get_control_pressed()},
                {"command pressed", get_command_pressed()},
                {"capslock active", get_capslock_active()},
                {"numlock active", get_numlock_active()}
            }
        );
        broadcast_event(
            get_root(), "hot key up", {
                {"string", get_key_string(key)},
                {"name", get_key_name(key)},
                {"shift pressed", get_shift_pressed()},
                {"alt pressed", get_alt_pressed()},
                {"control pressed", get_control_pressed()},
                {"command pressed", get_command_pressed()},
                {"capslock active", get_capslock_active()},
                {"numlock active", get_numlock_active()}
            }
        );
        broadcast_event(
            get_root(), "hot key press", {
                {"string", get_key_string(key)},
                {"name", get_key_name(key)},
                {"shift pressed", get_shift_pressed()},
                {"alt pressed", get_alt_pressed()},
                {"control pressed", get_control_pressed()},
                {"command pressed", get_command_pressed()},
                {"capslock active", get_capslock_active()},
                {"numlock active", get_numlock_active()}
            }
        );
        if (get_focused() == get_last_key_down())
        {
            broadcast_event(
                lookup_component( get_front_object() ), "key press", {
                    {"string", get_key_string(key)},
                    {"name", get_key_name(key)},
                    {"shift pressed", get_shift_pressed()},
                    {"alt pressed", get_alt_pressed()},
                    {"control pressed", get_control_pressed()},
                    {"command pressed", get_command_pressed()},
                    {"capslock active", get_capslock_active()},
                    {"numlock active", get_numlock_active()}
                }
            );
        }
    });
}

void broadcast_event (_COMPONENT& component, string registered_event, map<string, any> data)
{
    if (!is_event_registered(registered_event)) throw runtime_error("\nERROR: The event '" + registered_event + "' is not registered.");
    event* event_data = new event();
    for (pair item : data) event_data->set(item.first, item.second);
    component.run_callbacks(registered_event, event_data);
    delete event_data;
}

event::event ()
{
}

event::~event ()
{
}

void event::set (string key, any value)
{
    this->event_data[key] = value;
}

any event::get (string key)
{
    return this->event_data[key];
}

bool is_over_nothing ()
{
    return get_pixel_objects().size() == 0;
}

OBJECT_INSTANCE get_front_object ()
{
    return is_over_nothing() ? any_cast<OBJECT_INSTANCE>(get_root_pointer()->state("instance")) : get_pixel_objects()[0];
}

OBJECT_INSTANCE get_object_behind_object (OBJECT_INSTANCE query_object)
{
    vector<OBJECT_INSTANCE> objects = get_pixel_objects();
    int object_index = -1;
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i] == query_object)
        {
            object_index = i;
            break;
        }
    }
    return object_index < 0 || objects.size() < 2 ? new uint32_t(0xFFFFFFFF) : objects[object_index + 1];
}
