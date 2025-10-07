#!/bin/sh +x

CFLAGS=$(pkgconf --cflags sdl2 SDL2_mixer)
LIBS=$(pkgconf --libs sdl2 SDL2_mixer)

clang++ -o libengine.so -shared ./Engine/src/**/*.cpp ./Engine/src/LayerSystem/Layers/*.cpp ./dependencies/bin/imgui/*.cpp -I./Engine/src/ $CFLAGS -I./dependencies/bin/imgui/ -fPIC -std=c++17  $LIBS
clang++ ./Client/src/*/*.cpp ./dependencies/bin/imgui/*.cpp -I ./Client/src/ -I./Engine/src/ -I./dependencies/bin/imgui/ $CFLAGS ./libengine.so -fPIC -std=c++17 $LIBS --debug
