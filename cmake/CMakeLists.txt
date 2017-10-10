cmake_minimum_required(VERSION 2.6)

project (OptiCombi)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")
set (CMAKE_CXX_FLAGS "-O2")

include_directories(
    ../
)

add_executable(climbing_smtwtp_main
    ../climbing_smtwtp_main.cpp
    ../InstanceGenerator100.cpp
    ../InstanceGenerator1000.cpp
    ../Instance.cpp
    ../SMTWTP.cpp
    ../SMTWTP_initializer.cpp
    ../SMTWTP_climbing.cpp
)

add_executable(climbing_one_smtwtp_main
    ../climbing_one_smtwtp_main.cpp
    ../InstanceGenerator100.cpp
    ../InstanceGenerator1000.cpp
    ../Instance.cpp
    ../SMTWTP.cpp
    ../SMTWTP_initializer.cpp
    ../SMTWTP_vnd.cpp
    ../SMTWTP_climbing.cpp
)
