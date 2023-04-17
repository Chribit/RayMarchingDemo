// INCLUDE
#include "./memory.hpp"
#include "../shading/shading.hpp"
#include "../object/object.hpp"
#include "../colour/colour.hpp"
#include "../primitive/primitive.hpp"
#include "../instance/instance.hpp"

// CODE
void initialize_memory ()
{
    initialize_memory_type();
    initialize_push_constants();
    initialize_buffers();
    initialize_memory_allocation();
    bind_buffers();
}

void initialize_memory_type ()
{
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties( *get_hardware_device(), &memory_properties );

    #if DEBUG
        cout << "\nAvailable memory types:" << endl;
    #endif

    for (int i = 0; i < memory_properties.memoryTypeCount; i++)
    {
        VkMemoryType memory_type = memory_properties.memoryTypes[i];
        memory_flag_bits memory_property_bits = parse_memory_bits( memory_type.propertyFlags );
        uint32_t memory_heap_index = memory_type.heapIndex;
        VkMemoryHeap memory_heap = memory_properties.memoryHeaps[memory_heap_index];
        uint64_t memory_heap_size = memory_heap.size;
        memory_heap_flag_bits memory_heap_bits = parse_heap_bits( memory_heap.flags );

        #if DEBUG
            cout << "\tType index " << i << ":" << endl;
            cout << "\t\tHeap index " << memory_heap_index << ":" << endl;
            cout << "\t\t\tSize: " << memory_heap_size << " bytes" << endl;
            cout << "\t\t\tDevice local: " << (memory_heap_bits.device_local ? "true" : "false") << endl;
            cout << "\t\t\tMultiple instance: " << (memory_heap_bits.multi_instance ? "true" : "false") << endl;
            cout << "\t\tDevice local: " << (memory_property_bits.device_local ? "true" : "false") << endl;
            cout << "\t\tHost visible: " << (memory_property_bits.host_visible ? "true" : "false") << endl;
            cout << "\t\tHost coherent: " << (memory_property_bits.host_coherent ? "true" : "false") << endl;
            cout << "\t\tHost cached: " << (memory_property_bits.host_cached ? "true" : "false") << endl;
            cout << "\t\tLazily allocated: " << (memory_property_bits.lazily_allocated ? "true" : "false") << endl;
        #endif

        if (memory_property_bits.host_visible && memory_property_bits.host_coherent && memory_heap_size >= 1000000000)
        {
            set_buffer_memory_type_index(i);
            set_buffer_available_memory(memory_heap_size);
        }
    }

    #if DEBUG
        cout << "\nSelected memory types:" << endl;
        cout << "\tBuffer: " << get_buffer_memory_type_index() << endl;
    #endif
}

memory_flag_bits parse_memory_bits (VkMemoryPropertyFlags given_flags)
{
    int flags = given_flags;
    memory_flag_bits bits;
    if (flags - 16 >= 0)
    {
        bits.lazily_allocated = true;
        flags -= 16;
    }
    if (flags - 8 >= 0)
    {
        bits.host_cached = true;
        flags -= 8;
    }
    if (flags - 4 >= 0)
    {
        bits.host_coherent = true;
        flags -= 4;
    }
    if (flags - 2 >= 0)
    {
        bits.host_visible = true;
        flags -= 2;
    }
    if (flags - 1 >= 0)
    {
        bits.device_local = true;
        flags -= 1;
    }
    return bits;
}

memory_heap_flag_bits parse_heap_bits (VkMemoryHeapFlags given_flags)
{
    int flags = given_flags;
    memory_heap_flag_bits bits;
    if (flags - 2 >= 0)
    {
        bits.multi_instance = true;
        flags -= 2;
    }
    if (flags - 1 >= 0)
    {
        bits.device_local = true;
        flags -= 1;
    }
    return bits;
}

uint64_t determine_memory_size ()
{
    return (uint64_t) get_instance_buffer_size() + 
        get_primitive_buffer_size() + 
        get_blbihs_buffer_size() +
        get_tlbih_buffer_size();
}

void initialize_push_constants ()
{
    int width, height;
    get_window_size(&width, &height);
    push_constants constants = {
        .window_size = glm::ivec2(width, height),
        .scene_lower_bound = glm::vec2(0.0, 0.0),
        .scene_upper_bound = glm::vec2(0.0, 0.0),
        .camera_position = glm::vec2(0.0, 0.0),
        .primitive_count = 0,
        .camera_zoom = 1.0f
    };

    set_push_constants(constants);

    VkPushConstantRange constant_range;
    constant_range.stageFlags = set_shader_stage_bits({ .compute = true });
    constant_range.offset = 0;
    constant_range.size = sizeof(constants);

    set_push_constant_range(constant_range);
}

void initialize_buffers ()
{
    VkResult result = vkCreateBuffer(
        *get_hardware(),
        buffer_creation_info( get_maximum_instance_count_bytes(), set_buffer_bits({ .transfer_destination = true, .storage_buffer = true }), true),
        nullptr,
        get_instance_buffer()
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of instance buffer failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
    set_instance_buffer_size( get_maximum_instance_count_bytes() );

    result = vkCreateBuffer(
        *get_hardware(),
        buffer_creation_info( get_maximum_primitive_count_bytes(), set_buffer_bits({ .transfer_destination = true, .storage_buffer = true }), true),
        nullptr,
        get_primitive_buffer()
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of primitive buffer failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
    set_primitive_buffer_size( get_maximum_primitive_count_bytes() );

    result = vkCreateBuffer(
        *get_hardware(),
        buffer_creation_info( get_maximum_blbi_count_bytes(), set_buffer_bits({ .transfer_destination = true, .storage_buffer = true }), true),
        nullptr,
        get_blbihs_buffer()
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of bottom level bounding interval buffer failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
    set_blbihs_buffer_size( get_maximum_blbi_count_bytes() );

    result = vkCreateBuffer(
        *get_hardware(),
        buffer_creation_info( get_maximum_tlbi_count_bytes(), set_buffer_bits({ .transfer_destination = true, .storage_buffer = true }), true),
        nullptr,
        get_tlbih_buffer()
    );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Creation of top level bounding interval buffer failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
    set_tlbih_buffer_size( get_maximum_tlbi_count_bytes() );
}

void initialize_memory_allocation ()
{
    uint64_t combined_size = determine_memory_size();

    VkResult result = vkAllocateMemory(
        *get_hardware(),
        memory_allocation_info( combined_size, get_buffer_memory_type_index() ),
        nullptr,
        get_buffer_memory()
    );

    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Allocation of buffer memory failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
}

void bind_buffers ()
{
    uint32_t offset = 0;

    VkResult result = vkBindBufferMemory( *get_hardware(), *get_instance_buffer(), *get_buffer_memory(), offset );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Binding of instance buffer memory failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
    offset += get_instance_buffer_size();

    result = vkBindBufferMemory( *get_hardware(), *get_primitive_buffer(), *get_buffer_memory(), offset );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Binding of primitive buffer memory failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
    offset += get_primitive_buffer_size();

    result = vkBindBufferMemory( *get_hardware(), *get_blbihs_buffer(), *get_buffer_memory(), offset );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Binding of bottom level bounding interval buffer memory failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
    offset += get_blbihs_buffer_size();

    result = vkBindBufferMemory( *get_hardware(), *get_tlbih_buffer(), *get_buffer_memory(), offset );
    throw_error(result != VK_SUCCESS,
        "\n"
        "ERROR: Binding of top level bounding interval buffer memory failed.\n"
        "       The returned result is '" + get_result_string(result) + "'."
    );
}

uint32_t set_buffer_bits (memory_buffer_flag_bits given_bits)
{
    uint32_t bits = 0;
    if (given_bits.transfer_source) bits += 1;
    if (given_bits.transfer_destination) bits += 2;
    if (given_bits.uniform_texel_buffer) bits += 4;
    if (given_bits.storage_texel_buffer) bits += 8;
    if (given_bits.uniform_buffer) bits += 16;
    if (given_bits.storage_buffer) bits += 32;
    if (given_bits.index_buffer) bits += 64;
    if (given_bits.vertex_buffer) bits += 128;
    if (given_bits.indirect_buffer) bits += 256;
    return bits;
}

void transmit_object_data (VkCommandBuffer command_buffer)
{
    int maximum_copy_size = 65536;
    int buffer_size = 0;
    int iterations = 0;

    if ( get_instances_changed() )
    {
        buffer_size = get_instances()->size() * sizeof(instance);
        iterations = ceil( buffer_size / (float) maximum_copy_size );

        #if DEBUG
            cout << "        " << "Updating instances via " << iterations << " command" << (iterations > 1 ? "s." : ".") << endl;
        #endif

        for (int i = 0; i < iterations; i++)
        {
            vkCmdUpdateBuffer(
                command_buffer,
                *get_instance_buffer(),
                i * maximum_copy_size,
                i + 1 == iterations ? ((buffer_size - 1) % maximum_copy_size) + 1 : maximum_copy_size,
                get_instances()->data()
            );
        }

        set_instances_changed(false);
    }

    if ( get_primitives_changed() )
    {
        set_push_constant_primitive_count( get_primitives()->size() );

        buffer_size = get_primitives()->size() * sizeof(primitive);
        iterations = ceil( buffer_size / (float) maximum_copy_size );

        #if DEBUG
            cout << "        " << "Updating primitives via " << iterations << " command" << (iterations > 1 ? "s." : ".") << endl;
        #endif

        for (int i = 0; i < iterations; i++)
        {
            vkCmdUpdateBuffer(
                command_buffer,
                *get_primitive_buffer(),
                i * maximum_copy_size,
                i + 1 == iterations ? ((buffer_size - 1) % maximum_copy_size) + 1 : maximum_copy_size,
                get_primitives()->data()
            );
        }

        set_primitives_changed(false);
    }

    if ( get_blbihs_changed() )
    {
        buffer_size = get_blbihs()->size() * sizeof(bounding_interval);
        iterations = ceil( buffer_size / (float) maximum_copy_size );

        #if DEBUG
            cout << "        " << "Updating blbihs via " << iterations << " command" << (iterations > 1 ? "s." : ".") << endl;
        #endif

        for (int i = 0; i < iterations; i++)
        {
            vkCmdUpdateBuffer(
                command_buffer,
                *get_blbihs_buffer(),
                i * maximum_copy_size,
                i + 1 == iterations ? ((buffer_size - 1) % maximum_copy_size) + 1 : maximum_copy_size,
                get_blbihs()->data()
            );
        }

        set_blbihs_changed(false);
    }

    if ( get_tlbih_changed() )
    {
        construct_tlbih();

        buffer_size = get_tlbih()->size() * sizeof(bounding_interval);
        iterations = ceil( buffer_size / (float) maximum_copy_size );

        #if DEBUG
            cout << "        " << "Updating tlbih via " << iterations << " command" << (iterations > 1 ? "s." : ".") << endl;
        #endif

        for (int i = 0; i < iterations; i++)
        {
            vkCmdUpdateBuffer(
                command_buffer,
                *get_tlbih_buffer(),
                i * maximum_copy_size,
                i + 1 == iterations ? ((buffer_size - 1) % maximum_copy_size) + 1 : maximum_copy_size,
                get_tlbih()->data()
            );
        }

        set_tlbih_changed(false);
    }
}

void cleanup_memory ()
{
    vkDestroyBuffer( *get_hardware(), *get_instance_buffer(), nullptr );
    vkDestroyBuffer( *get_hardware(), *get_primitive_buffer(), nullptr );
    vkDestroyBuffer( *get_hardware(), *get_blbihs_buffer(), nullptr );
    vkDestroyBuffer( *get_hardware(), *get_tlbih_buffer(), nullptr );
    vkFreeMemory( *get_hardware(), *get_buffer_memory(), nullptr );
}
