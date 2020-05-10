#pragma once

#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */

// Guard against double gl.h/gl3.h includes
#define __gl_h_
#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#include <gl/gl3.h>
#include <gl/gl3ext.h>
#endif
