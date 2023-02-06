// INCLUDE
#include "./main.hpp"


// CODE
int main() {

    try 
    {
        create_interface("Application", []() {

            // set camera zoom
            set_camera_state({0.0, 0.0}, 0.125);

            // create pink circle shape to showcase basic shape
            COMPONENT pink_circle = component("pink_circle").add(
                &shape("pink_circle").radius(1.0f).red(249).green(9).blue(145).build()
            ).position({3.0, 6.0});

            // create blue rounded square shape to showcase rounding
            COMPONENT blue_rounded_square = component("blue_rounded_square").add(
                &shape("blue_rounded_square").width(3.0f).height(2.0f).round(0.2f).red(9).green(221).blue(249).build()
            ).position({-1.0, -4.0});

            // create moon shape to showcase subtraction / difference operation
            COMPONENT moon = component("moon").add(
                &shape("moon").radius(1.5f).red(253).green(255).blue(214).subtract(
                    &shape("moon_stencil").radius(1.25f).x(0.35f).build()
                ).build()
            ).position({-3.0, 2.0});

            // create vector of components to iterate over
            static vector<_COMPONENT> shapes = {
                pink_circle,
                blue_rounded_square,
                moon
            };

            // draw line
            line("ray").from({-12.0, 0.0}).to({100.0, 0.0}).thickness(0.02f).radius(0.01f).draw();

            // update line to follow cursor
            on_mouse_move([](vec2 position) {

                // init ray origin
                vec2 ray_origin = get_line("ray").from();

                // determine line direction from cursor position minus origin position
                vec2 direction = glm::normalize(position - ray_origin);

                // initialise variables
                string circle_id;
                vec2 current_position = ray_origin;
                float distance;
                float closest_distance = 1000000.0;

                // march along ray
                for (int i = 1; i < 64; i++)
                {
                    // terminate if closest distance below termination threshold 0.001
                    if (closest_distance < 0.001f) return;

                    // reset closest distance to some large distance outside of screen space
                    closest_distance = 1000000.0;

                    // determine closest signed distance to scene
                    // in this case, simply iterate over shapes
                    for (int j = 0; j < shapes.size(); j++)
                    {
                        // get distance to shape
                        distance = get_primitive_signed_distance(current_position - shapes[j].position(), j);

                        // if smaller than current smallest distance, store new smallest distance
                        closest_distance = distance < closest_distance ? distance : closest_distance;
                    }

                    // take step along ray of size closest distance
                    current_position += direction * closest_distance;
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