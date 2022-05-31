#pragma once
#ifdef EMSCRIPTEN
#include <GLES3/gl3.h>
#include <emscripten/html5_webgl.h>
#else
#include "glr_windows.h"
#endif