cmake_minimum_required( VERSION 2.8 )

## Load CMAKE configuration from environment variables
set( CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH} )
set( CMAKE_PREFIX_PATH $ENV{CMAKE_PREFIX_PATH} )

## cross-platform stuff
set(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc) #change these to -w32- if needed
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
set(CMAKE_RANLIB i686-w64-mingw32-ranlib)

## Specify a project name
project( CameraInput )

## Set the build type
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE "Release"
       CACHE STRING "Build configuration 'Release' or 'Debug'."
       FORCE )
endif()

## Install the executable to <install_directory>/bin
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
## Install libraries (if any) to <install_directory>/bin
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(LIB_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(LIB_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(LIB_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

## add library include dir
include_directories( LIB_INSTALL_INCLUDE_DIR )
## add library headers dir
include_directories( ${PROJECT_SOURCE_DIR}/src/opencvimagereader )

add_subdirectory(${PROJECT_SOURCE_DIR}/src bin)
