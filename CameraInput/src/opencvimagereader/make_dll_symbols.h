/*
Copyright 2019 HackRats

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

// This file is not currently used. Manual alternative to using CMakeLists GenerateExportHeader to generate bin/opencvimagereader/opencvimagereader_Export.h

#pragma once

// Define EXPORTED for any platform
#ifdef opencvimagereader_BUILT_AS_STATIC
#  define opencvimagereader_EXPORT
#  define OPENCVIMAGEREADER_NO_EXPORT
#else
  #if 1 // Switch to "if 0" to make a Linux .so instead of a Windows .dll
    #ifdef WIN_EXPORT
      // Exporting...
      #ifdef __GNUC__
        #define EXPORTED __attribute__ ((dllexport))
      #else
        #define EXPORTED __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
      #endif
    #else
      #ifdef __GNUC__
        #define EXPORTED __attribute__ ((dllimport))
      #else
        #define EXPORTED __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
      #endif
    #endif
    #define NOT_EXPORTED
  #else
    #if __GNUC__ >= 4
      #define EXPORTED __attribute__ ((visibility ("default")))
      #define NOT_EXPORTED  __attribute__ ((visibility ("hidden")))
    #else
      #define EXPORTED
      #define NOT_EXPORTED
    #endif
  #endif
#endif
