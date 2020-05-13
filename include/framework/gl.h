#pragma once

#if defined(__APPLE__)
// Guard against double gl.h/gl3.h includes
#define __gl_h_
#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#elif defined(__EMSCRIPTEN__)
#include<emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif
