// INCLUDE
#include "./main.hpp"


// CODE
int main() {

    try 
    {
        create_interface("Application", []() {
            LINE ray = line("ray").from({-0.5, -0.5}).to({0.5, 0.5}).radius(0.0f).draw();

            static bool active = false;

            on_key_down([](int key) {
                if (key == 340) active = true;
            });

            on_key_up([](int key) {
                active = false;
            });

            on_mouse_scroll([](vec2 position, dvec2 offset) {
                get_line("ray").radius( glm::clamp(float(get_line("ray").radius() + sign(offset.y) * 0.001), 0.0f, get_line("ray").thickness() / 2.0f) ).draw();
            });

            on_mouse_move([](vec2 position) {
                if (!active) get_line("ray").to(position).draw();
                else get_line("ray").from(position).draw();
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