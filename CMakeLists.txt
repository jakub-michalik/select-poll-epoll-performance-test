cmake_minimum_required(VERSION 3.10)
project(SelectPollEpollPerformanceTest C)
set(CMAKE_C_STANDARD 99)

# Executable files
add_executable(selectTest1 selectTest1.c)
add_executable(selectTest2 selectTest2.c)
add_executable(pollTest1 pollTest1.c)
add_executable(epollTest1 epollTest1.c)
