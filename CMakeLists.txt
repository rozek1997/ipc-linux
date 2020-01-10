cmake_minimum_required(VERSION 3.13)
project(so_app C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "-Wall -lpthread" )



add_executable(so_app app/main.c app/processes.c app/so_project.h app/signals.c app/shared_memory.c app/fifo.c app/semaphores.c)
add_executable(so_client client/client.c)

