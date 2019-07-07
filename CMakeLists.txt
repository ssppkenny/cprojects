cmake_minimum_required(VERSION 3.10)
project(avltree C)


set(CMAKE_C_STANDARD 99)
find_package(PythonInterp 3.6 REQUIRED)

find_package(PythonLibs 3.6 REQUIRED)

include_directories(avltree ${PYTHON_INCLUDE_DIRS})
add_executable(avltree avltree.c)
target_link_libraries(avltree ${PYTHON_LIBRARIES})


