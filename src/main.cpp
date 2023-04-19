// INCLUDE
#include "./main.hpp"


// CODE
int main() {

    try 
    {
        create_interface("Ray Marching Demonstration", {1024, 512}, []() {

            // set camera zoom
            set_camera_state({0.0, 0.0}, 0.125);

            // create pink circle shape to showcase basic shape
            component("pink_circle").add(
                &shape("pink_circle").radius(1.0f).red(249).green(9).blue(145).build()
            ).position({3.0, 6.0});

            // create blue rounded square shape to showcase rounding
            component("blue_rounded_square").add(
                &shape("blue_rounded_square").width(3.0f).height(2.0f).round(0.2f).red(9).green(221).blue(249).build()
            ).position({-1.0, -4.0});

            // create moon shape to showcase subtraction / difference operation
            component("moon").add(
                &shape("moon").radius(1.5f).red(253).green(255).blue(214).subtract(
                    &shape("moon_stencil").radius(1.25f).x(0.35f).build()
                ).build()
            ).position({-3.0, 2.0});

            // create vector of components to iterate over
            static vector<string> shape_ids = {
                "pink_circle",
                "blue_rounded_square",
                "moon"
            };

            // draw line
            line("ray").from({-12.0, 0.0}).to({12.0, 0.0}).thickness(0.05f).radius(0.01f).draw();

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
                float distance;
                float closest_distance = 1000000.0;

                // initialise flags
                // !sphere_tracing = fixed step ray marching
                // sphere_tracing = sphere tracing
                // sphere_tracing && enhanced = enhanced sphere tracing
                bool sphere_tracing = true;
                bool enhanced = false;

                // initialise variables relevant to fixed step ray marching
                float step_size = 0.2f;

                // initialise variables relevant to enhanced sphere tracing
                float relaxation_factor = 1.6f;
                bool circles_overlap = false;
                vec2 previous_position;
                float previous_position_radius;

                // march along ray
                for (int i = 0; i < 64; i++)
                {
                    // terminate if closest distance below termination threshold 0.001
                    if (closest_distance < 0.001f) return;

                    // reset closest distance to some large distance outside of screen space
                    closest_distance = 1000000.0;

                    // determine closest signed distance to scene
                    // in this case, simply iterate over shapes
                    for (int j = 0; j < shape_ids.size(); j++)
                    {
                        // get distance to shape
                        distance = get_primitive_signed_distance(current_position - get_component(shape_ids.at(j)).position(), j);

                        // if smaller than current smallest distance, store new smallest distance
                        closest_distance = distance < closest_distance ? distance : closest_distance;
                    }

                    // if closest distance is negative = inside a shape AND we are using fixed step size ray marching
                    // halve step size
                    if (closest_distance < 0 && !sphere_tracing) step_size /= 2.0f;

                    // if using enhanced sphere tracing
                    if (sphere_tracing && enhanced)
                    {
                        // determine if previous step circle overlaps current step circle
                        circles_overlap = (glm::length(current_position - previous_position) - closest_distance - previous_position_radius) <= 0;

                        // if not overlapping, jump back to previous position and revert to classical sphere tracing (relaxation_factor = 1)
                        if (!circles_overlap)
                        {
                            current_position = previous_position;
                            relaxation_factor = 1.0f;
                        }
                        // update relevant variables if overlapping
                        else
                        {
                            previous_position = current_position;
                            previous_position_radius = closest_distance;
                        }
                    }

                    // take step along ray of size closest distance
                    // if not using sphere tracing, use fixed step size ray marching approach
                    // if enhanced flag set in addition to using sphere tracing, use relaxation factor
                    current_position += direction * (sphere_tracing ? closest_distance : step_size) * (sphere_tracing && enhanced ? relaxation_factor : 1.0f);
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