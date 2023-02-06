// INCLUDE
#include "./synchronisation.hpp"
#include "../swapchain/swapchain.hpp"

// CODE
void initialize_synchronisation ()
{
    vector<VkSemaphore> image_available_semaphores = *get_image_available_semaphores();
    vector<VkSemaphore> image_rendered_semaphores = *get_image_rendered_semaphores();
    vector<VkFence> image_presented_fences = *get_image_presented_fences();

    image_available_semaphores.resize( *get_swapchain_image_count() );
    image_rendered_semaphores.resize( *get_swapchain_image_count() );
    image_presented_fences.resize( *get_swapchain_image_count() );

    VkResult result;

    for (int i = 0; i < *get_swapchain_image_count(); i++)
    {
        result = vkCreateSemaphore( *get_hardware(), semaphore_creation_info(), nullptr, &image_available_semaphores[i]);
        throw_error(result != VK_SUCCESS,
            "\n"
            "ERROR: Creation of 'image available' semaphore for swapchain frame " + to_string(i) + " failed.\n"
            "       The returned result is '" + get_result_string(result) + "'."
        );
        
        result = vkCreateSemaphore( *get_hardware(), semaphore_creation_info(), nullptr, &image_rendered_semaphores[i]);
        throw_error(result != VK_SUCCESS,
            "\n"
            "ERROR: Creation of 'image rendered' semaphore for swapchain frame " + to_string(i) + " failed.\n"
            "       The returned result is '" + get_result_string(result) + "'."
        );

        result = vkCreateFence( *get_hardware(), fence_creation_info(), nullptr, &image_presented_fences[i]);
        throw_error(result != VK_SUCCESS,
            "\n"
            "ERROR: Creation of 'image presented' fence for swapchain frame " + to_string(i) + " failed.\n"
            "       The returned result is '" + get_result_string(result) + "'."
        );
    }

    set_image_available_semaphores(image_available_semaphores);
    set_image_rendered_semaphores(image_rendered_semaphores);
    set_image_presented_fences(image_presented_fences);
}

void await_render_completion (int swapchain_frame)
{
    VkResult result = vkWaitForFences(
        *get_hardware(),
        1,
        get_cpu_image_presented(swapchain_frame),
        true,
        get_synchronisation_timeout()
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Waiting for 'render completion' fence of swapchain frame " + to_string(swapchain_frame) + " failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
}

void reset_fence (int swapchain_frame)
{
    VkResult result = vkResetFences(
        *get_hardware(),
        1,
        get_cpu_image_presented(swapchain_frame)
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Resetting of 'render completion' fence for swapchain frame " + to_string(swapchain_frame) + " failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
}

void cleanup_synchronisation ()
{
    vector<VkSemaphore> image_available_semaphores = *get_image_available_semaphores();
    vector<VkSemaphore> image_rendered_semaphores = *get_image_rendered_semaphores();
    vector<VkFence> image_presented_fences = *get_image_presented_fences();

    for (int i = 0; i < *get_swapchain_image_count(); i++)
    {
        vkDestroySemaphore( *get_hardware(), image_available_semaphores[i], nullptr);
        vkDestroySemaphore( *get_hardware(), image_rendered_semaphores[i], nullptr);
        vkDestroyFence( *get_hardware(), image_presented_fences[i], nullptr);
    }
}
