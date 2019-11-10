cmake_minimum_required(VERSION 2.8)
project( opencvimagereader C CXX )

set(SOURCE_FILES
    opencvimagereader.h
    opencvimagereader.cpp
)

add_library(opencvimagereader SHARED ${SOURCE_FILES})

## dll stuff
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER( opencvimagereader
             BASE_NAME opencvimagereader
             EXPORT_MACRO_NAME opencvimagereader_EXPORT
             EXPORT_FILE_NAME opencvimagereader_Export.h
             STATIC_DEFINE opencvimagereader_BUILT_AS_STATIC
)
install(TARGETS opencvimagereader DESTINATION ${LIB_INSTALL_LIB_DIR})
install(FILES opencvimagereader.h DESTINATION ${LIB_INSTALL_INCLUDE_DIR})
