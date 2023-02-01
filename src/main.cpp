// INCLUDE
#include "./main.hpp"


// CODE
int main() {

    try 
    {
        create_interface("Application", []() {

            // set camera zoom
            set_camera_state({0.0, 0.0}, 0.125);

            // create clover shape
            SHAPE clover_leaf_1 = shape("clover-leaf-1").radius(0.5f).x(-0.5f).green(147).blue(19);
            SHAPE clover_leaf_2 = shape("clover-leaf-2").radius(0.5f).x(0.5f).green(147).blue(19);
            SHAPE clover_leaf_3 = shape("clover-leaf-3").radius(0.5f).y(-0.5f).green(147).blue(19);
            SHAPE clover_leaf_4 = shape("clover-leaf-4").radius(0.5f).y(0.5f).green(147).blue(19);
            SHAPE clover_shape = shape("clover").add(&clover_leaf_1).add(&clover_leaf_2).add(&clover_leaf_3).add(&clover_leaf_4).build();

            // create shape components
            COMPONENT clover = component("clover-component").add(&clover_shape).rotation(0, 45.0f).position(0, {3.0, 7.0});

            // draw line
            LINE ray = line("ray").from({-12.0, 0.0}).thickness(0.02f).radius(0.01f).draw();

            // update line to follow cursor
            on_mouse_move([](vec2 position) {

                // init ray origin
                vec2 ray_origin = get_line("ray").from();

                // determine line direction from cursor position minus origin position
                vec2 direction = glm::normalize(position - ray_origin);

                // update target position of line and draw
                get_line("ray").to(ray_origin + direction * 100.0f).draw();

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