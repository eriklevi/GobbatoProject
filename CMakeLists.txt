cmake_minimum_required(VERSION 3.6)
project(Material_assignment)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    input/Co_100_1_NT_0.txt
    input/Co_100_1_NT_1.txt
    input/Co_100_1_NT_2.txt
    input/Co_100_1_NT_3.txt
    input/Co_100_1_NT_4.txt
    input/Co_100_1_NT_5.txt
    input/Co_100_1_NT_6.txt
    input/Co_100_1_NT_7.txt
    input/Co_100_1_NT_8.txt
    input/Co_100_1_NT_9.txt
    input/Co_100_1_T_0.txt
    input/Co_100_1_T_1.txt
    input/Co_100_1_T_2.txt
    input/Co_100_1_T_3.txt
    input/Co_100_1_T_4.txt
    input/Co_100_1_T_5.txt
    input/Co_100_1_T_6.txt
    input/Co_100_1_T_7.txt
    input/Co_100_1_T_8.txt
    input/Co_100_1_T_9.txt
    input/Co_100_20_NT_0.txt
    input/Co_100_20_NT_1.txt
    input/Co_100_20_NT_2.txt
    input/Co_100_20_NT_3.txt
    input/Co_100_20_NT_4.txt
    input/Co_100_20_NT_5.txt
    input/Co_100_20_NT_6.txt
    input/Co_100_20_NT_7.txt
    input/Co_100_20_NT_8.txt
    input/Co_100_20_NT_9.txt
    input/Co_100_20_T_0.txt
    input/Co_100_20_T_1.txt
    input/Co_100_20_T_2.txt
    input/Co_100_20_T_3.txt
    input/Co_100_20_T_4.txt
    input/Co_100_20_T_5.txt
    input/Co_100_20_T_6.txt
    input/Co_100_20_T_7.txt
    input/Co_100_20_T_8.txt
    input/Co_100_20_T_9.txt
    input/Co_30_1_NT_0.txt
    input/Co_30_1_NT_1.txt
    input/Co_30_1_NT_2.txt
    input/Co_30_1_NT_3.txt
    input/Co_30_1_NT_4.txt
    input/Co_30_1_NT_5.txt
    input/Co_30_1_NT_6.txt
    input/Co_30_1_NT_7.txt
    input/Co_30_1_NT_8.txt
    input/Co_30_1_NT_9.txt
    input/Co_30_1_T_0.txt
    input/Co_30_1_T_1.txt
    input/Co_30_1_T_2.txt
    input/Co_30_1_T_3.txt
    input/Co_30_1_T_4.txt
    input/Co_30_1_T_5.txt
    input/Co_30_1_T_6.txt
    input/Co_30_1_T_7.txt
    input/Co_30_1_T_8.txt
    input/Co_30_1_T_9.txt
    input/Co_30_20_NT_0.txt
    input/Co_30_20_NT_1.txt
    input/Co_30_20_NT_2.txt
    input/Co_30_20_NT_3.txt
    input/Co_30_20_NT_4.txt
    input/Co_30_20_NT_5.txt
    input/Co_30_20_NT_6.txt
    input/Co_30_20_NT_7.txt
    input/Co_30_20_NT_8.txt
    input/Co_30_20_NT_9.txt
    input/Co_30_20_T_0.txt
    input/Co_30_20_T_1.txt
    input/Co_30_20_T_2.txt
    input/Co_30_20_T_3.txt
    input/Co_30_20_T_4.txt
    input/Co_30_20_T_5.txt
    input/Co_30_20_T_6.txt
    input/Co_30_20_T_7.txt
    input/Co_30_20_T_8.txt
    input/Co_30_20_T_9.txt
    input/link_to_300.txt
    CMakeLists.txt
    heuristic.cpp
    heuristic.h
    main.cpp
    solveAll.sh
    utils.h)

add_executable(Material_assignment ${SOURCE_FILES})