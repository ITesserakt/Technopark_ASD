cmake_minimum_required(VERSION 3.21)
project(Technopark_ASD)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        rapidcheck
        GIT_REPOSITORY https://github.com/emil-e/rapidcheck.git)
FetchContent_MakeAvailable(rapidcheck)

link_libraries(rapidcheck)

add_executable(1-1 1-1.cpp)
add_executable(1-2 1-2.cpp)
add_executable(1-3 1-3.cpp)
add_executable(1-4 1-4.cpp)
add_executable(2-2 2-2.cpp)
add_executable(2-3 2-3.cpp)
add_executable(2-4 2-4.cpp)
add_executable(3-1 3-1.cpp)
add_executable(3-2 3-2.cpp)
add_executable(3-3 3-3.cpp)
add_executable(4-1 4-1.cpp)
add_executable(4-2 4-2.cpp)
add_executable(4-3 4-3.cpp)
add_executable(5-3 5-3.cpp)
add_executable(6 6.cpp)
add_executable(7-2 7-2.cpp)