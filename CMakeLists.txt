cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0091 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GLM_ENABLE_CXX_17 ON CACHE BOOL "Enable C++17 for GLM")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$CONFIG:Debug>Debug>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$CONFIG:Release>:Release")

project(Falcon-PBR)

add_subdirectory("vendor/glfw-3.4")
add_subdirectory("vendor/glm-1.0.0")
add_subdirectory("vendor/glad")
add_subdirectory("vendor/stb_image")
add_subdirectory("vendor/tiny_gltf")

add_executable(Falcon-PBR "main.cpp" "include/shader.h" "src/shader.cpp" "include/openglErrorReporting.h" "src/openglErrorReporting.cpp" "include/vertexBuffer.h" "src/vertexBuffer.cpp"
	"include/indexBuffer.h"  "include/vertexArray.h" "src/vertexArray.cpp" "include/vertexBufferLayout.h" "src/vertexBufferLayout.cpp" "include/camera.h" "src/camera.cpp" "include/iCameraController.h" 
	"include/fpsCameraController.h" "src/fpsCameraController.cpp" "include/gltfScene.h" "src/gltfScene.cpp" "include/texture.h" "src/texture.cpp" "include/material.h" "src/material.cpp" 
	"include/gltfTextureCache.h" "src/gltfTextureCache.cpp" "include/renderer.h" "src/renderer.cpp" "include/lightComponent.h" "src/lightComponent.cpp")

#if(MSVC)
#    set_property(TARGET Falcon-PBR PROPERTY
#        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#endif()

add_custom_command(TARGET Falcon-PBR POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources" "$<TARGET_FILE_DIR:Falcon-PBR>/resources")
# This is useful to get an ASSETS_PATH in your IDE during development but you should comment this if you compile a release version and uncomment the next line
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
# Uncomment this line to setup the ASSETS_PATH macro to the final assets directory when you share the project
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/")

file(GLOB_RECURSE PROJECT_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

target_include_directories(Falcon-PBR PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_sources(Falcon-PBR PUBLIC "${PROJECT_SRC}")

target_link_libraries(Falcon-PBR PRIVATE glfw glad glm stb_image tiny_gltf)