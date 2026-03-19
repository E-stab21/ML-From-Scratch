cmake_minimum_required(VERSION 3.31)
project(ML-From-Scratch)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(libs/eigen-5.0.0)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()

add_executable(ML-From-Scratch src/main.cpp
        src/classes/Node.h
        src/classes/Layer.h
        src/classes/Network.h
        src/classes/MatrixNetwork.h
        src/classes/MatrixNetwork.cpp
        src/classes/Network.cpp
        src/classes/Node.cpp
        src/classes/Layer.cpp)

target_include_directories(ML-From-Scratch PRIVATE libs/eigen-5.0.0/Eigen)




