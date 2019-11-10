cmake_minimum_required(VERSION 2.8)
project( opencvimagereader C CXX )

set(SOURCE_FILES
    opencvimagereader.h
    opencvimagereader.cpp
)

add_library(opencvimagereader.dll SHARED ${SOURCE_FILES})
set_target_properties(opencvimagereader.dll PROPERTIES
        PREFIX ""
        SUFFIX ""
#        LINK_FLAGS "-Wl,--add-stdcall-alias" #should be required but broken
        POSITION_INDEPENDENT_CODE 1 # this is to avoid MinGW warning; 
        # MinGW generates position-independent-code for DLL by default
)

## dll stuff
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER( opencvimagereader.dll
             BASE_NAME opencvimagereader
             EXPORT_MACRO_NAME opencvimagereader_EXPORT
             EXPORT_FILE_NAME opencvimagereader_Export.h
             STATIC_DEFINE opencvimagereader_BUILT_AS_STATIC
)

install(TARGETS opencvimagereader.dll DESTINATION ${LIB_INSTALL_LIB_DIR})
install(FILES opencvimagereader.h DESTINATION ${LIB_INSTALL_INCLUDE_DIR})
