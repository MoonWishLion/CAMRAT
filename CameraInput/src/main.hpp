#ifdef SHARED_LIB_SOURCE
#define PUBLIC_FUNCTION __stdcall __declspec(dllexport)
#else
#define PUBLIC_FUNCTION __stdcall __declspec(dllimport)
#endif

#include <iostream>
#include "opencv2/opencv.hpp"

#pragma once

#ifdef cplusplus
extern "C" {
#endif

#ifdef SHARED_LIB_SOURCE
#define PUBLIC_FUNCTION stdcall  declspec(dllexport)
#else
#define PUBLIC_FUNCTION stdcall declspec(dllimport)
#endif

PUBLIC_FUNCTION void calibrate();
PUBLIC_FUNCTION float getPosFB();
PUBLIC_FUNCTION float getPosLR();
PUBLIC_FUNCTION void stopThread();
PUBLIC_FUNCTION void startThread(bool showWindow);
PUBLIC_FUNCTION void runLoop(bool showWindow);

#ifdef cplusplus
}
#endif