// INCLUDE
#include "./main.hpp"


// CODE
int main() {

    try 
    {
        create_interface("Application", []() {

            // set camera zoom
            set_camera_state({0.0, 0.0}, 0.125);

            // create shapes for scene
            COMPONENT pink_circle = component("pink_circle").add(
                &shape("pink_circle").radius(1.0f).red(249).green(9).blue(145).build()
            ).position({3.0, 6.0});
            COMPONENT blue_rounded_square = component("blue_rounded_square").add(
                &shape("blue_rounded_square").width(3.0f).height(2.0f).round(0.2f).red(9).green(221).blue(249).build()
            ).position({-1.0, -4.0});

            // create vector of components to iterate over
            static vector<_COMPONENT> shapes = {
                pink_circle,
                blue_rounded_square
            };

            // draw line
            line("ray").from({-12.0, 0.0}).to({100.0, 0.0}).thickness(0.02f).radius(0.01f).draw();

            // create circle shapes
            shape("circle-0");
            for (int i = 1; i < 16; i++) get_shape("circle-0").add( &shape("circle-" + to_string(i)) );
            get_shape("circle-0").build();

            // update line to follow cursor
            on_mouse_move([](vec2 position) {

                // init ray origin
                vec2 ray_origin = get_line("ray").from();

                // determine line direction from cursor position minus origin position
                vec2 direction = glm::normalize(position - ray_origin);

                // update target position of line and draw
                get_line("ray").to(ray_origin + direction * 100.0f).draw();

                // initialise variables
                string circle_id;
                vec2 current_position = ray_origin;
                float distance;
                float closest_distance = 100.0;

                // delete old circle 0
                for (int i = 0; i < 16; i++) get_shape("circle-" + to_string(i)).delete_shape();

                // // get closest distance from current position for new circle 0
                // for (int j = 0; j < shapes.size(); j++)
                // {
                //     distance = get_primitive_signed_distance(current_position - shapes[j].position(), j);
                //     closest_distance = distance < closest_distance ? distance : closest_distance;
                // }

                // create new circle 0 at ray origin
                shape("circle-0").x(current_position.x).y(current_position.y).radius(closest_distance).thickness(0.02f).red(255).green(255).blue(255);

                // for (int i = 1; i < 16; i++) get_shape("circle-0").add( &shape("circle-" + to_string(i)) );

                // // set next position along ray
                // current_position += direction * closest_distance;

                // // march along ray
                // for (int i = 1; i < 16; i++)
                // {
                //     // terminate if closest distance below termination threshold
                //     if (closest_distance < 0.001f) return;

                //     // reset closest distance
                //     closest_distance = 100.0;

                //     // determine new circle id
                //     circle_id = "circle-" + to_string(i);

                //     // iterate over primitives in scene
                //     for (int j = 0; j < shapes.size(); j++)
                //     {
                //         distance = get_primitive_signed_distance(current_position - shapes[j].position(), j);
                //         closest_distance = distance < closest_distance ? distance : closest_distance;
                //     }

                //     // delete old circle shape with given id
                //     get_shape(circle_id)->delete_shape();

                //     // add new circle shape with given id
                //     new_circles.add(
                //         &shape(circle_id).x(current_position.x).y(current_position.y).radius(closest_distance).thickness(0.02f).red(255).green(255).blue(255)
                //     );

                //     // otherwise, take step along ray
                //     current_position += direction * closest_distance;
                // }

                // get_shape("circle-0").build();

                // // update circle shapes along ray
                // get_component("ray-circles").replace(0, &new_circles.build());

            });


            // static bool active = false;

            // on_key_down([](int key) {
            //     if (key == 340) active = true;
            // });

            // on_key_up([](int key) {
            //     active = false;
            // });

            // on_mouse_scroll([](vec2 position, dvec2 offset) {
            //     get_line("ray").radius( glm::clamp(float(get_line("ray").radius() + sign(offset.y) * 0.001), 0.0f, get_line("ray").thickness() / 2.0f) ).draw();
            // });
        });
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}