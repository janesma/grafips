// Copyright (C) Intel Corp.  2014.  All Rights Reserved.

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice (including the
// next paragraph) shall be included in all copies or substantial
// portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//  **********************************************************************/
//  * Authors:
//  *   Mark Janes <mark.a.janes@intel.com>
//  **********************************************************************/

#include "test/test_gpu_context.h"

#include <GL/gl.h>

#include <stdio.h>
#include <string.h>
#include <vector>

using Grafips::MockContext;

static const char * vshader =
    "attribute vec2 coord2d;\n"
    "varying vec2 v_TexCoordinate;\n"
    "void main(void) {\n"
    "  gl_Position = vec4(coord2d.x, coord2d.y, 0, 1);\n"
    "  v_TexCoordinate = vec2(coord2d.x, coord2d.y);\n"
    "}";

static const char * fshader =
    "uniform sampler2D texUnit;\n"
    "varying vec2 v_TexCoordinate;\n"
    "void main(void) {\n"
    "  gl_FragColor = texture2D(texUnit, v_TexCoordinate);\n"
    "}";

static void
CheckError(const char * file, int line) {
  const int error = glGetError();
  if ( error == GL_NO_ERROR)
    return;
  printf("ERROR: %x %s:%i\n", error, file, line);
  exit(-1);
}

#define GL_CHECK() CheckError(__FILE__, __LINE__)

static void
PrintCompileError(GLint shader) {
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_TRUE)
    return;
  static const int MAXLEN = 1024;
  std::vector<char> log(MAXLEN);
  GLsizei len;
  glGetShaderInfoLog(shader,  MAXLEN,  &len, log.data());
  printf("ERROR -- compile failed: %s\n", log.data());
}


MockContext::MockContext() {
  const int32_t init_attrs[] = {
    WAFFLE_PLATFORM, WAFFLE_PLATFORM_GLX,
    0,
  };
  waffle_init(init_attrs);

  m_dpy = waffle_display_connect(NULL);
  const int32_t config_attrs[] = {
    WAFFLE_CONTEXT_API, WAFFLE_CONTEXT_OPENGL,
    0,
  };

  m_config = waffle_config_choose(m_dpy, config_attrs);
  m_window = waffle_window_create(m_config, 320, 320);
  m_ctx = waffle_context_create(m_config, NULL);
  waffle_make_current(m_dpy, m_window, m_ctx);

  glGenBuffers(1, &vbo);
  GL_CHECK();
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  GL_CHECK();

  const int vs = glCreateShader(GL_VERTEX_SHADER);
  GL_CHECK();
  int len = strlen(vshader);
  glShaderSource(vs, 1, &vshader, &len);
  GL_CHECK();
  glCompileShader(vs);
  PrintCompileError(vs);

  const int fs = glCreateShader(GL_FRAGMENT_SHADER);
  GL_CHECK();
  len = strlen(fshader);
  glShaderSource(fs, 1, &fshader, &len);
  GL_CHECK();
  glCompileShader(fs);
  PrintCompileError(fs);

  prog = glCreateProgram();
  glAttachShader(prog, vs);
  GL_CHECK();
  glAttachShader(prog, fs);
  GL_CHECK();
  glLinkProgram(prog);
  GL_CHECK();

  attribute_coord2d = glGetAttribLocation(prog,  "coord2d");
  GL_CHECK();

  tex_uniform = glGetUniformLocation(prog, "texUnit");
  GL_CHECK();
    
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  GL_CHECK();
  std::vector<GLfloat> data = { 0, 0, 0, 1, 1, 1};
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat),
               data.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  GL_CHECK();

  glEnable(GL_TEXTURE_2D);
  GL_CHECK();

  glGenTextures(1, &texture);  
  GL_CHECK();
  glBindTexture(GL_TEXTURE_2D, texture);
  GL_CHECK();
  glActiveTexture(GL_TEXTURE0);
  GL_CHECK();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  GL_CHECK();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  GL_CHECK();

    unsigned char tex_data[] {
      255, 0, 0, 0,
          0, 255, 0, 0,
          0, 0, 255, 0,
          0, 0, 0, 255,
          };
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
               2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
  GL_CHECK();

  glBindTexture(GL_TEXTURE_2D, 0);
  GL_CHECK();

  waffle_window_show(m_window);
}

MockContext::~MockContext() {
  waffle_context_destroy(m_ctx);
  waffle_window_destroy(m_window);
  waffle_config_destroy(m_config);
  waffle_display_disconnect(m_dpy);
}

void
MockContext::Draw() {
  glEnable(GL_TEXTURE_2D);
  GL_CHECK();

  glClearColor(1, 1, 1, 1);
  GL_CHECK();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_CHECK();

  glUseProgram(prog);
  GL_CHECK();

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  GL_CHECK();

  glEnableVertexAttribArray(attribute_coord2d);
  GL_CHECK();
  glVertexAttribPointer(attribute_coord2d,   // attribute
                        2,                   // number of elements
                        // per vertex, here
                        // (x,y)
                        GL_FLOAT,            // the type of each element
                        GL_FALSE,            // take our values as-is
                        0,                   // no space between values
                        0);                  // use the vertex buffer object
  GL_CHECK();

  glActiveTexture(GL_TEXTURE0);
  GL_CHECK();
  glBindTexture(GL_TEXTURE_2D, texture);
  GL_CHECK();

  glUniform1i(tex_uniform, 0);
  GL_CHECK();
  
  // 3 vertices in vbo
  glDrawArrays(GL_TRIANGLES, 0, 3);
  GL_CHECK();
  glDisableVertexAttribArray(attribute_coord2d);
  GL_CHECK();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  GL_CHECK();

  waffle_window_swap_buffers(m_window);
  GL_CHECK();
}

