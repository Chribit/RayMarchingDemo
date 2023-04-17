rm -rf "/Users/chribit/Documents/Github/RayMarchingDemo/out"
mkdir "/Users/chribit/Documents/Github/RayMarchingDemo/out"

cp -a "/Users/chribit/Documents/Github/RayMarchingDemo/lib/graphics/mac/shading" "/Users/chribit/Documents/Github/RayMarchingDemo/out/shading"

/usr/bin/g++ \
    -std=c++17 \
    -O0 \
    -fsanitize=address \
    -g \
    /Users/chribit/Documents/Github/RayMarchingDemo/src/main.cpp \
    /Users/chribit/Documents/Github/RayMarchingDemo/lib/graphics/mac/graphics.a \
    /Users/chribit/Documents/Github/RayMarchingDemo/lib/interface/mac/interface.a \
    -o \
    /Users/chribit/Documents/Github/RayMarchingDemo/out/ray_marching_demo \
    -L \
    /Users/chribit/VulkanSDK/1.3.236.0/macOS/lib \
    /Users/chribit/VulkanSDK/1.3.236.0/macOS/lib/libvulkan.1.dylib \
    /Users/chribit/VulkanSDK/1.3.236.0/macOS/lib/libvulkan.1.3.236.dylib \
    -L \
    /usr/local/Cellar/glfw/3.3.7/lib/ \
    -lglfw