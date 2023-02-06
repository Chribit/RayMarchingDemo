set engine_folder=C:/Users/Kronark/Documents/GitHub/Graphics/src/engine

RMDIR /S /Q "%engine_folder%/image/out_dev/"

MKDIR "%engine_folder%/image/out_dev/"

C:/msys64/mingw64/bin/g++.exe ^
    -std=c++17 ^
    -Wall ^
    -O0 ^
    -fsanitize=address ^
    -g ^
    -c ^
    %engine_folder%/image/image.cpp ^
    -o ^
    %engine_folder%/image/out_dev/image.o ^
    -L ^
    C:/VulkanSDK/1.3.236.0/Lib ^
    -l:vulkan-1.lib ^
    -L ^
    C:/Users/Kronark/Documents/Lib/glfw/lib-mingw-w64 ^
    -l:libglfw3.a ^
    -lgdi32 ^
    -I ^
    C:/VulkanSDK/1.3.236.0/Include ^
    -I ^
    C:/Users/Kronark/Documents/Lib/glm ^
    -I ^
    C:/Users/Kronark/Documents/Lib/glfw/include ^
    -DDEBUG=true ^
    -DFOR_MAC=false ^
    -DFOR_WIN=true ^
    -DFOR_LIN=false

C:/msys64/mingw64/bin/g++.exe ^
    -std=c++17 ^
    -Wall ^
    -O0 ^
    -fsanitize=address ^
    -g ^
    -c ^
    %engine_folder%/image/image.access.cpp ^
    -o ^
    %engine_folder%/image/out_dev/image.access.o ^
    -L ^
    C:/VulkanSDK/1.3.236.0/Lib ^
    -l:vulkan-1.lib ^
    -L ^
    C:/Users/Kronark/Documents/Lib/glfw/lib-mingw-w64 ^
    -l:libglfw3.a ^
    -lgdi32 ^
    -I ^
    C:/VulkanSDK/1.3.236.0/Include ^
    -I ^
    C:/Users/Kronark/Documents/Lib/glm ^
    -I ^
    C:/Users/Kronark/Documents/Lib/glfw/include ^
    -DDEBUG=true ^
    -DFOR_MAC=false ^
    -DFOR_WIN=true ^
    -DFOR_LIN=false

cd C:/Users/Kronark/Documents/GitHub/Graphics/script

build_windows_dev.bat

cd C:/Users/Kronark/Documents/GitHub/Graphics/src/engine/image
