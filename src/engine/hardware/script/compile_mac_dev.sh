engine_folder="/Users/chribit/Documents/Github/Graphics/src/engine"

rm -rf "$engine_folder/hardware/out_dev/"

mkdir "$engine_folder/hardware/out_dev/"

/usr/bin/g++ \
    -std=c++17 \
    -Wall \
    -O0 \
    -fsanitize=address \
    -g \
    -c \
    $engine_folder/hardware/hardware.cpp \
    -o \
    $engine_folder/hardware/out_dev/hardware.o \
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
    $engine_folder/hardware/hardware.access.cpp \
    -o \
    $engine_folder/hardware/out_dev/hardware.access.o \
    -DDEBUG=true \
    -DFOR_MAC=true \
    -DFOR_WIN=false \
    -DFOR_LIN=false

cd /Users/chribit/Documents/Github/Graphics/script/

sh build_mac_dev.sh

cd /Users/chribit/Documents/Github/Graphics/src/engine/hardware/