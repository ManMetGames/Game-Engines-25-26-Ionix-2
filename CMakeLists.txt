cmake_minimum_required(VERSION 3.20)
project(GameEngines25-26)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Dependencies paths
set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/dependencies/bin)

# Engine Library
file(GLOB_RECURSE ENGINE_SOURCES 
    "Engine/src/*.cpp"
    "Engine/src/*.hpp"
    "Engine/src/*.h"
)

add_library(Engine SHARED ${ENGINE_SOURCES})

target_include_directories(Engine PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine/src
    ${VENDOR_DIR}/SDL/SDL2-2.30.6/include
    ${VENDOR_DIR}/SDL/SDL2_image/include
    ${VENDOR_DIR}/SDL/SDL_TTF/include
    ${VENDOR_DIR}/SDL/SDL2_mixer-2.8.0/include
    ${VENDOR_DIR}/lua/include
    ${VENDOR_DIR}/ImGui
    ${VENDOR_DIR}/box2d/include/box2d
    ${VENDOR_DIR}/sol2/include
)

target_link_directories(Engine PUBLIC
    ${VENDOR_DIR}/SDL/SDL2-2.30.6/lib/x64
    ${VENDOR_DIR}/SDL/SDL2_image/lib
    ${VENDOR_DIR}/SDL/SDL_TTF/lib
    ${VENDOR_DIR}/SDL/SDL2_mixer-2.8.0/lib/x64
    ${VENDOR_DIR}/lua
    ${VENDOR_DIR}/box2d/lib
)

target_link_libraries(Engine
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_ttf
    lua54
    box2d
)

target_compile_definitions(Engine PRIVATE
    ENGINE_EXPORTS
    ENGINE_BUILD_DLL
)

# Client Executable
file(GLOB_RECURSE CLIENT_SOURCES 
    "Client/src/*.cpp"
    "Client/src/*.h"
)

add_executable(Client ${CLIENT_SOURCES})

target_include_directories(Client PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine/src
    ${VENDOR_DIR}/SDL/SDL2-2.30.6/include
    ${VENDOR_DIR}/SDL/SDL2_image/include
    ${VENDOR_DIR}/SDL/SDL_TTF/include
    ${VENDOR_DIR}/lua/include
    ${VENDOR_DIR}/ImGui
    ${VENDOR_DIR}/box2d/include/box2d
    ${VENDOR_DIR}/sol2/include
)

target_link_libraries(Client
    Engine
    SDL2
    SDL2_image
    SDL2_ttf
    lua54
    box2d
)

# Copy DLLs to output directory (Windows)
if(WIN32)
    add_custom_command(TARGET Client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${VENDOR_DIR}/SDL/SDL2-2.30.6/lib/x64/SDL2.dll"
            $<TARGET_FILE_DIR:Client>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${VENDOR_DIR}/SDL/SDL2_image/lib/SDL2_image.dll"
            $<TARGET_FILE_DIR:Client>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${VENDOR_DIR}/SDL/SDL2_mixer-2.8.0/lib/x64/SDL2_mixer.dll"
            $<TARGET_FILE_DIR:Client>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${VENDOR_DIR}/SDL/SDL_TTF/lib/SDL2_ttf.dll"
            $<TARGET_FILE_DIR:Client>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${VENDOR_DIR}/lua/lua54.dll"
            $<TARGET_FILE_DIR:Client>
    )
    
    # Copy Scripts directory
    add_custom_command(TARGET Client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Client/Scripts"
            "$<TARGET_FILE_DIR:Client>/Scripts"
    )
endif()

# Set Client as startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Client)
