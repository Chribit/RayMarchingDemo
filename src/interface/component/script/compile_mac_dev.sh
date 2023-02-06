interface_folder="/Users/chribit/Documents/Github/Interface/src/interface"

rm -rf "$interface_folder/component/out_dev/"

mkdir "$interface_folder/component/out_dev/"

/usr/bin/g++ \
    -std=c++17 \
    -Wall \
    -O0 \
    -fsanitize=address \
    -g \
    -c \
    $interface_folder/component/component.cpp \
    -o \
    $interface_folder/component/out_dev/component.o \
    -DDEBUG=true \
    -DFOR_MAC=true \
    -DFOR_WIN=false \
    -DFOR_LIN=false

cd /Users/chribit/Documents/Github/Interface/script/

sh build_mac_dev.sh

cd /Users/chribit/Documents/Github/Interface/src/interface/component/