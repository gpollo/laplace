cmake_minimum_required(VERSION 3.0)

project (laplace)

find_package(Threads REQUIRED)

set(SOURCES
    src/main.c
    src/lattice.c
    src/worker.c
    src/problem.c
)

add_executable(laplace ${SOURCES} )

add_library(problem SHARED ${SOURCES})

target_link_libraries (laplace ${CMAKE_THREAD_LIBS_INIT} m)
