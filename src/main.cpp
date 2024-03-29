// INCLUDE
#include "./main.hpp"


// CODE
vec2 get_primitive_position (string shape_id)
{
    return get_component(shape_id).position();
}

float get_primitive_rotation (string shape_id)
{
    return radians( get_component(shape_id).rotation() );
}

vec2 translate_point_for_primitive (string shape_id, vec2 point)
{
    return point - get_primitive_position(shape_id);
}

vec2 rotate_point_for_primitive (string shape_id, vec2 point)
{
    return mat2x2( cos( get_primitive_rotation(shape_id) ), -1 * sin( get_primitive_rotation(shape_id) ), sin( get_primitive_rotation(shape_id) ), cos( get_primitive_rotation(shape_id) ) ) * point;  
}

float get_primitive_signed_distance (string shape_id, int shape_index, vec2 point)
{
    // translate and rotate point
    point = translate_point_for_primitive(shape_id, point);
    point = rotate_point_for_primitive(shape_id, point);

    // get signed distance
    float signed_distance = get_primitive_signed_distance(point, shape_index);
    
    // return distance
    return signed_distance;
}

float get_shortest_distance (vector<string>* shape_ids, vec2 current_position)
{
    float distance;
    float closest_distance = 100.0;
    
    // iterate over shapes
    for (int j = 0; j < shape_ids->size(); j++)
    {
        // get distance to shape
        distance = get_primitive_signed_distance(shape_ids->at(j), j, current_position);

        // if smaller than current smallest distance, store new smallest distance
        closest_distance = distance < closest_distance ? distance : closest_distance;
    }

    return closest_distance;
}

void update_circle (int index, vec2 position, float radius)
{
    // determine circle shape id
    string circle_id = "circle-" + to_string(index);

    // delete old circle (interface library object)
    _SHAPE old_circle = get_shape(circle_id);
    get_shapes()->erase(circle_id);

    // replace old circle with new circle
    get_component("circles").replace(index, 
        &shape(circle_id).x(position.x).y(position.y).radius(radius).thickness(0.025).visible(radius != 0.0).build()
    );

    // delete old circle (graphics engine object)
    old_circle.delete_shape(false);
}

void set_info_text_position ()
{
    int width, height;
    glfwGetWindowSize(get_window(), &width, &height);
    float screen_space_unit = height / 2.0f;

    float x = -1.0 * (((width / screen_space_unit) / 2.0) / 0.125) + 0.5;
    float y = -1.0 * (1.0 / 0.125) + 3.0;

    get_component("info").position({x, y});
}


// MAIN
int main() {

    try 
    {
        create_interface("Ray Marching Demonstration", {1024, 512}, []() {

            // set camera zoom
            set_camera_state({0.0, 0.0}, 0.125);

            // create pink circle shape to showcase basic shape
            component("pink_circle").add(
                &shape("pink_circle").radius(1.5f).red(249).green(9).blue(145).build()
            ).position({1.0, 7.0});

            // create blue rounded square shape to showcase rounding
            component("blue_rounded_rectangle").add(
                &shape("blue_rounded_square").width(3.0f).height(2.0f).round(0.4f).red(9).green(221).blue(249).build()
            ).position({7.0, -6.0});

            // create yellow rotated square shape to showcase what happens at angles
            component("yellow_rectangle").add(
                &shape("yellow_rectangle").width(2.0f).height(2.0f).red(252).green(233).blue(27).build()
            ).position({0.0, -1.5}).rotation(45.0f);

            // create pink slab to showcase overstepping along a ray
            component("pink_slab").add(
                &shape("pink_slab").width(0.75f).height(5.0f).red(240).green(10).blue(252).build()
            ).position({-5.0, 4.0}).rotation(10.0f);

            // create vector of components to iterate over
            static vector<string> shape_ids = {
                "pink_circle",
                "blue_rounded_rectangle",
                "yellow_rectangle",
                "pink_slab"
            };

            // draw line
            line("ray").from({12.0, 0.0}).to({-12.0, 0.0}).thickness(0.05f).radius(0.01f).draw();

            // create component to contain shapes for each circle
            component("circles");
            for (int i = 0; i < 64; i++)
            {
                get_component("circles").add(
                    &shape("circle-" + to_string(i)).build()
                );
            }

            // initialise flags
            static bool naive = true;
            static bool sphere = false;
            static bool enhanced = false;

            // listen for keyboard events
            get_root().listen("hot key press", [](event* data) {

                if (get_string(data, "name") == "one")
                {
                    naive = true;
                    sphere = false;
                    enhanced = false;
                }
                if (get_string(data, "name") == "two")
                {
                    naive = false;
                    sphere = true;
                    enhanced = false;
                }
                if (get_string(data, "name") == "three")
                {
                    naive = false;
                    sphere = false;
                    enhanced = true;
                }
            });

            // insert info text regarding hot keys
            initialise_font({255, 255, 255, 255});
            text(
                "info",
                u"Hot Keys\n1 = Naive Ray Marching\n2 = Sphere Tracing\n3 = Enhanced Sphere Tracing\n    1.5 relaxation factor",
                {255, 255, 255, 255}
            ).scale(0.5f);
            set_info_text_position();

            // make text stick to bottom left corner
            get_root().listen("window resize", [](event* data) {
                set_info_text_position();
            });

            // update line to follow cursor
            on_mouse_move([](vec2 position) {

                // init ray origin
                vec2 ray_origin = get_line("ray").from();

                // determine line direction from cursor position minus origin position
                vec2 direction = glm::normalize(position - ray_origin);

                // update ray direction
                get_line("ray").to(ray_origin + direction * 100.0f).draw();

                // initialise variables
                vec2 current_position = ray_origin;
                float closest_distance;

                // initialise variables relevant to fixed step ray marching
                float step_size = 1.0f;

                // initialise variables relevant to enhanced sphere tracing
                float relaxation_factor = 1.5f;
                bool circles_overlap = false;
                vec2 previous_position = ray_origin;
                float previous_position_radius = 0.0f;

                // march along ray
                for (int i = 0; i < 64; i++)
                {
                    // determine closest signed distance in scene
                    closest_distance = get_shortest_distance(&shape_ids, current_position);

                    // if using enhanced sphere tracing
                    if (enhanced)
                    {
                        // determine if previous step circle overlaps current step circle
                        circles_overlap = (glm::length(current_position - previous_position) - closest_distance - previous_position_radius) <= 0;

                        // if not overlapping:
                        // 1. jump back to previous position
                        // 2. revert to naive ray marching
                        // 3. reevaluate distance
                        if (!circles_overlap)
                        {
                            current_position = previous_position + direction * previous_position_radius;
                            relaxation_factor = 1.0f;
                            closest_distance = get_shortest_distance(&shape_ids, current_position);
                        }

                        // update relevant variables if overlapping
                        previous_position = current_position;
                        previous_position_radius = closest_distance;
                    }

                    // update circle for step
                    update_circle(i, current_position, naive ? step_size : closest_distance);

                    // take step along ray of size closest distance
                    // if performing enhanced sphere tracing, factor in relaxation factor
                    current_position += direction * ((naive ? step_size : closest_distance) * (enhanced ? relaxation_factor : 1.0f));

                    // terminate if closest distance below termination threshold 0.001
                    // terminate if closest distance is to large (and naive ray marching is not used)
                    if (closest_distance < 0.001f || closest_distance > 11.0 && !naive)
                    {
                        // replace remaining circles that did not get redrawn with "null" circles
                        for (int j = i; j < 64; j++) update_circle(j, {0.0, 0.0}, 0.0);
                        return;
                    }
                }
            
            });
        });
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}