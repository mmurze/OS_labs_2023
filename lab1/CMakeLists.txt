project(daemon)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Werror)

add_executable(${PROJECT_NAME} src/main.cpp src/daemon.cpp src/config.cpp)
