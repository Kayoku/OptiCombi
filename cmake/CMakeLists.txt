cmake_minimum_required(VERSION 2.6)

project (OptiCombi)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")
set (CMAKE_CXX_FLAGS "-O2")

include_directories(
    ../
)

add_executable(oc
    ../super_main.cpp
    ../InstanceGenerator100.cpp
    ../InstanceGenerator1000.cpp
    ../Instance.cpp
    ../SMTWTP.cpp
    ../SMTWTP_initializer.cpp
    ../SMTWTP_vnd.cpp
    ../SMTWTP_climbing.cpp
    ../SMTWTP_ILS.cpp
    ../SMTWTP_vns.cpp
)
