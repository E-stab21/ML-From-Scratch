cmake_minimum_required(VERSION 3.31)
project(ML-From-Scratch)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(libs/eigen-5.0.0)

add_executable(ML-From-Scratch main.cpp
        Classes/Node.h
        Classes/Layer.h
        Classes/Network.h
        MatrixNetwork.h
        MatrixNetwork.cpp
        Classes/Network.cpp
        Classes/Node.cpp
        Classes/Layer.cpp)

target_include_directories(ML-From-Scratch PRIVATE libs/eigen-5.0.0/Eigen)




