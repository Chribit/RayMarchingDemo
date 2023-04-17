// INCLUDE
#include "./engine.hpp"

// DATA
static struct {
    string name;
    string application_name;
    int swapchain_frame;
    uint64_t frame_time;
    bool initialising = true;
    vector<OBJECT_INSTANCE> pixel_objects;
} engine_data;

// CODE
string get_engine_name ()
{
    return engine_data.name;
}

void set_engine_name (string name)
{
    engine_data.name = name;
}

string get_application_name ()
{
    return engine_data.application_name;
}

void set_application_name (string name)
{
    engine_data.application_name = name;
}

int get_swapchain_frame ()
{
    return engine_data.swapchain_frame;
}

void set_swapchain_frame (int swapchain_frame)
{
    engine_data.swapchain_frame = swapchain_frame;
}

uint64_t get_frame_time ()
{
    return engine_data.frame_time;
}

void set_frame_time (uint64_t frame_time)
{
    engine_data.frame_time = frame_time;
}

bool get_initialising ()
{
    return engine_data.initialising;
}

void set_initialising (bool initialising)
{
    engine_data.initialising = initialising;
}

void set_engine_state (engine_state state)
{
    set_instances( state.instances );
    set_instance_pointers( state.instance_pointers );
    set_object_pointers( state.objects );
    set_primitives( state.primitives );
    set_instance_aabbs( state.instance_aabbs );
    set_tlbih( state.tlbih );
}

engine_state get_engine_state ()
{
    engine_state state;
    state.instances = *get_instances();
    state.instance_pointers = *get_instance_pointers();
    state.objects = *get_object_pointers();
    state.primitives = *get_primitives();
    state.instance_aabbs = *get_instance_aabbs();
    state.tlbih = *get_tlbih();
    return state;
}

glm::vec2 get_mouse_coordinates ()
{
    double x_pos;
    double y_pos;
    glfwGetCursorPos( get_window(), &x_pos, &y_pos);
    glm::ivec2 xy = {x_pos, y_pos};

    glm::ivec2 coordinates = {(int) xy.x, (int) xy.y};

    glm::ivec2 window_size;
    get_window_size(&window_size.x, &window_size.y);

    glm::ivec2 y_inverted_coordinates = glm::ivec2( coordinates.x, window_size.y - coordinates.y );

    glm::vec2 window_size_float = {(float) window_size.x, (float) window_size.y};

    glm::vec2 point;
    point.x = (2 * y_inverted_coordinates.x - window_size.x) / window_size_float.y;
    point.y = (2 * y_inverted_coordinates.y - window_size.y) / window_size_float.y;

    point = transform_point_for_camera(point);

    return point;
}

void set_pixel_objects (vector<OBJECT_INSTANCE> pixel_objects)
{
    sort(pixel_objects.begin(), pixel_objects.end(), [pixel_objects] (uint32_t* a, uint32_t* b) {
        return get_instance_depth(a) > get_instance_depth(b);
    });
    engine_data.pixel_objects = pixel_objects;
}

vector<OBJECT_INSTANCE> get_pixel_objects ()
{
    return engine_data.pixel_objects;
}
