engine_folder="/Users/chribit/Documents/Github/Graphics/src/engine"

rm -rf "$engine_folder/window/out_dev/"

mkdir "$engine_folder/window/out_dev/"

/usr/bin/g++ \
    -std=c++17 \
    -Wall \
    -O0 \
    -fsanitize=address \
    -g \
    -c \
    $engine_folder/window/window.cpp \
    -o \
    $engine_folder/window/out_dev/window.o \
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
    $engine_folder/window/window.access.cpp \
    -o \
    $engine_folder/window/out_dev/window.access.o \
    -DDEBUG=true \
    -DFOR_MAC=true \
    -DFOR_WIN=false \
    -DFOR_LIN=false

cd /Users/chribit/Documents/Github/Graphics/script/

sh build_mac_dev.sh

cd /Users/chribit/Documents/Github/Graphics/src/engine/window/