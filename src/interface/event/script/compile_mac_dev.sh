interface_folder="/Users/chribit/Documents/Github/Interface/src/interface"

rm -rf "$interface_folder/event/out_dev/"

mkdir "$interface_folder/event/out_dev/"

/usr/bin/g++ \
    -std=c++17 \
    -Wall \
    -O0 \
    -fsanitize=address \
    -g \
    -c \
    $interface_folder/event/event.cpp \
    -o \
    $interface_folder/event/out_dev/event.o \
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
    $interface_folder/event/event.access.cpp \
    -o \
    $interface_folder/event/out_dev/event.access.o \
    -DDEBUG=true \
    -DFOR_MAC=true \
    -DFOR_WIN=false \
    -DFOR_LIN=false

cd /Users/chribit/Documents/Github/Interface/script/

sh build_mac_dev.sh

cd /Users/chribit/Documents/Github/Interface/src/interface/event/