cmake_minimum_required(VERSION 3.7)
project(Light)
set(OpenCV_DIR /usr/local/share/OpenCV)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Vec3.cpp Vec3.h Camera.cpp Camera.h Scene.cpp Scene.h Object/Object.cpp Object/Object.h Object/Sphere.cpp Object/Sphere.h Light.cpp Light.h)
add_executable(Light ${SOURCE_FILES})
target_link_libraries(Light ${OpenCV_LIBS})