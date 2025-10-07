#!/bin/sh +x

CFLAGS=$(pkgconf --cflags sdl2 SDL2_mixer)
LIBS=$(pkgconf --libs sdl2 SDL2_mixer)

clang++ -o libengine.so -shared ./Engine/src/**/*.cpp ./Engine/src/LayerSystem/Layers/*.cpp ./Engine/src/Architecture/ECS/*.cpp ./Engine/src/Architecture/AudioSystem/*.cpp ./dependencies/bin/imgui/*.cpp -I./Engine/src/ $CFLAGS -I./dependencies/bin/imgui/ -fPIC -std=c++17  $LIBS
clang++ ./Client/src/*/*.cpp ./dependencies/bin/imgui/*.cpp -I ./Client/src/ -I./Engine/src/ -I./dependencies/bin/imgui/ $CFLAGS ./libengine.so -fPIC -std=c++17 $LIBS --debug

if [ $? -eq 0 ]; then
    echo "Build succeeded.. Renaming a.out"
    mv ./a.out ./client
else
    echo "Build failed"
fi
