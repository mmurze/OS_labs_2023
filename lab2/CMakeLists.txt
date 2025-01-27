project(lab2)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Werror)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)
find_library(LIBRT rt)

include_directories(host client connections utils)
file(GLOB HOST host/*.cpp)
file(GLOB CLIENT client/*.cpp)
file(GLOB UTILS utils/*.cpp)
file(GLOB CONNECTIONS connections/*.cpp)

foreach(PATH_TO_CON ${CONNECTIONS})
    get_filename_component(FILE_NAME ${PATH_TO_CON} NAME_WLE)
    string(REGEX REPLACE "conn_" "" CONN_NAME ${FILE_NAME})

    add_executable(host_${CONN_NAME}
        ${HOST} ${CLIENT} ${UTILS}
        connections/conn.h connections/conn_${CONN_NAME}.cpp connections/conn_${CONN_NAME}.h)

    target_link_libraries(host_${CONN_NAME} PUBLIC pthread rt)
endforeach()
