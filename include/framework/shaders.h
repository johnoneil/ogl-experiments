#pragma once

#include <framework/gl.h>

GLuint LoadShaderFromFiles(const char * vertex_file_path, const char * const fragment_file_path);
GLuint LoadShaderFromSource(const char * const vertex_file_path,const char * const fragment_file_path);


