cmake_minimum_required(VERSION 3.2)

project(CameraInput)

SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
set(CMAKE_RANLIB i686-w64-mingw32-ranlib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

target_link_libraries(shared_lib.dll)

add_library(shared_lib.dll SHARED main.cpp main.hpp)
set_target_properties(shared_lib.dll PROPERTIES
        PREFIX ""
        SUFFIX ""
        LINK_FLAGS "-Wl,--add-stdcall-alias"
        POSITION_INDEPENDENT_CODE 0 # this is to avoid MinGW warning; MinGW generates position-independent-code for DLL by default
        )