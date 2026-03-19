# Install script for directory: C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ML_From_Scratch")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Users/ethan/AppData/Local/Programs/CLion/bin/mingw/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE FILE FILES
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/AdolcForward"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/AlignedVector3"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/ArpackSupport"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/AutoDiff"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/BVH"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/EulerAngles"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/FFT"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/IterativeSolvers"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/KroneckerProduct"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/LevenbergMarquardt"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/MatrixFunctions"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/MPRealSupport"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/NNLS"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/NonLinearOptimization"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/NumericalDiff"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/OpenGLSupport"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/Polynomials"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/SparseExtra"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/SpecialFunctions"
    "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/Splines"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE DIRECTORY FILES "C:/Users/ethan/Projects/ML-From-Scratch/libs/eigen-5.0.0/unsupported/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/ethan/Projects/ML-From-Scratch/cmake-build-debug/libs/eigen-5.0.0/unsupported/Eigen/CXX11/cmake_install.cmake")

endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/ethan/Projects/ML-From-Scratch/cmake-build-debug/libs/eigen-5.0.0/unsupported/Eigen/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
