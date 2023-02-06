// INCLUDE
#include "./engine.hpp"


// CODE
void start_engine (string engine_name, string application_name, uint32_t width, uint32_t height)
{
    set_engine_name(engine_name);
    set_application_name(application_name);
    set_initial_window_size({width, height});
    initialize();
    run();
    cleanup();
}

void initialize ()
{
    initialize_window();
    initialize_hardware();
    initialize_swapchain(false);
    initialize_command();
    initialize_memory();
    initialize_descriptor();
    initialize_shaders();
    initialize_synchronisation();

    glfwSetCursorPosCallback( get_window(), [](GLFWwindow* window, double xpos, double ypos) {
        run_mouse_move_callbacks();
    });

    glfwSetMouseButtonCallback( get_window(), [](GLFWwindow* window, int button, int action, int mods) {
        if (action == GLFW_PRESS) run_mouse_down_callbacks();
        else if (action == GLFW_RELEASE) run_mouse_up_callbacks();
    });

    glfwSetScrollCallback( get_window(), [](GLFWwindow* window, double x_offset, double y_offset) {
        // BUG: invert y offset to match window coordinates
        run_mouse_scroll_callbacks({x_offset, y_offset});
    });

    glfwSetCursorEnterCallback( get_window(), [](GLFWwindow* window, int entered) {
        if (entered) run_mouse_enter_callbacks();
        else run_mouse_leave_callbacks();
    });

    glfwSetWindowSizeCallback( get_window(), [](GLFWwindow* window, int width, int height) {
        run_window_resize_callbacks();
    });

    glfwSetKeyCallback( get_window(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) run_key_down_callbacks(key);
        else if (action == GLFW_RELEASE) run_key_up_callbacks(key);
    });

    run_initialisation_callbacks();
}

void run ()
{
    set_swapchain_frame(0);

    while ( !glfwWindowShouldClose( get_window() ) )
    {   
        glfwWaitEvents();
        render();
        determine_pixel_object();
    }

    vkDeviceWaitIdle( *get_hardware());
}

void render ()
{
    await_render_completion( get_swapchain_frame() );

    if (!get_initialising())
    {
        #if DEBUG
            cout << "\tRunning render completion callbacks..." << endl;
        #endif

        run_render_completion_callbacks();

        #if DEBUG
            cout << "\tRender completion callbacks finished." << endl;
        #endif
    }

    set_initialising(false);

    #if DEBUG
        set_frame_time( (uint64_t) duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() );
        cout << "\nRendering swapchain frame: " << get_swapchain_frame() << endl;
        cout << "\tAcquiring next swapchain image..." << endl;
    #endif

    acquire_next_swapchain_image( get_swapchain_frame() );

    #if DEBUG
        cout << "\tNext swapchain image acquired." << endl;
        cout << "\tSwapchain image index: " << *get_swapchain_image_index() << endl;
        cout << "\tResetting fence..." << endl;
    #endif

    reset_fence( get_swapchain_frame() );

    #if DEBUG
        cout << "\tFence reset." << endl;
        cout << "\tRecording commands..." << endl;
    #endif

    record_command_buffer( get_swapchain_frame() );

    #if DEBUG
        cout << "\tCommands recorded." << endl;
        cout << "\tSubmitting commands..." << endl;
    #endif

    submit_command_buffer( get_swapchain_frame() );

    #if DEBUG
        cout << "\tCommands submitted." << endl;
        cout << "\tPresenting image to window..." << endl;
    #endif

    present_image( get_swapchain_frame(), get_swapchain_image_index() );

    #if DEBUG
        cout << "\tImage presented to window." << endl;
        cout << "\tWaiting for render to complete..." << endl;
    #endif

    set_swapchain_frame( (get_swapchain_frame() + 1) % *get_swapchain_image_count() );

    #if DEBUG
        float seconds = (((uint64_t) duration_cast<microseconds>(system_clock::now().time_since_epoch()).count()) - get_frame_time()) / 1000000.0;
        cout << "\tRender completed in " << setprecision(8) << fixed << seconds << " seconds." << endl;
    #endif
}

void set_camera_state (glm::vec2 position, float zoom)
{
    set_push_constant_camera_position(position);
    set_push_constant_camera_zoom(zoom);
}

void cleanup ()
{
    cleanup_synchronisation();
    cleanup_shaders();
    cleanup_descriptor();
    cleanup_memory();
    cleanup_command();
    cleanup_swapchain();
    cleanup_hardware();
    cleanup_window();
    cleanup_vulkan();
    cleanup_objects();

    #if DEBUG
        cout << endl;
    #endif
}
