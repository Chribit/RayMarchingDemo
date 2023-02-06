engine_folder="/Users/chribit/Documents/Github/Graphics/src/engine"

rm -rf "$engine_folder/swapchain/out_dev/"

mkdir "$engine_folder/swapchain/out_dev/"

/usr/bin/g++ \
    -std=c++17 \
    -Wall \
    -O0 \
    -fsanitize=address \
    -g \
    -c \
    $engine_folder/swapchain/swapchain.cpp \
    -o \
    $engine_folder/swapchain/out_dev/swapchain.o \
    -DDEBUG=true \
    -DFOR_MAC=true \
    -DFOR_WIN=false \
    -DFOR_LIN=false

/usr/bin/g++ \
    -std=c++17 \
    -Wall \
    -O0 \
    -fsanitize=address \
    -g \
    -c \
    $engine_folder/swapchain/swapchain.access.cpp \
    -o \
    $engine_folder/swapchain/out_dev/swapchain.access.o \
    -DDEBUG=true \
    -DFOR_MAC=true \
    -DFOR_WIN=false \
    -DFOR_LIN=false

cd /Users/chribit/Documents/Github/Graphics/script/

sh build_mac_dev.sh

cd /Users/chribit/Documents/Github/Graphics/src/engine/swapchain/