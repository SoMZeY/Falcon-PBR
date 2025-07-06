cmake_minimum_required(VERSION 3.16)
project(tiny_gltf)

add_library(tiny_gltf STATIC "src/tiny_gltf_loader.cpp")

target_sources(tiny_gltf PRIVATE "src/tiny_gltf_loader.cpp")
target_include_directories(tiny_gltf PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

# Link the stb_image library that provides STB_IMAGE_IMPLEMENTATION
target_link_libraries(tiny_gltf PUBLIC stb_image)