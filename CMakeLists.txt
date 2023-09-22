cmake_minimum_required(VERSION 3.10)
project(SelectPollEpollPerformanceTest C)
set(CMAKE_C_STANDARD 99)
add_executable(selectTest1 selectTest1.c)

# Adding the second executable (selectTest2)
add_executable(selectTest2 selectTest2.c)
