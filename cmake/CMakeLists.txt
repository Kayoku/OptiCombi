cmake_minimum_required(VERSION 2.6)

project (OptiCombi)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")

include_directories(
    ../
)

add_executable(simple_smtwtp_main 
    ../simple_smtwtp_main.cpp
    ../InstanceGenerator.cpp
    ../Instance.cpp
    ../SMTWTP.cpp
    ../SMTWTP_simple.cpp
)

add_executable(climbing_smtwtp_main
    ../climbing_smtwtp_main.cpp
    ../InstanceGenerator.cpp
    ../Instance.cpp
    ../SMTWTP.cpp
    ../SMTWTP_simple.cpp
    ../SMTWTP_climbing.cpp
)
