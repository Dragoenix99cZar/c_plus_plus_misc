cmake_minimum_required(VERSION 3.10)
project(StandalonePrograms C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Automatically find and add all C++ files as individual executables
file(GLOB SRCS_CPP "*.cpp")
foreach(file ${SRCS_CPP})
    get_filename_component(target_name ${file} NAME_WE)
    add_executable(${target_name} ${file})
endforeach()

# Automatically find and add all C files as individual executables
file(GLOB SRCS_C "*.c")
foreach(file ${SRCS_C})
    get_filename_component(target_name ${file} NAME_WE)
    add_executable(${target_name} ${file})
endforeach()