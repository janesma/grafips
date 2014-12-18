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

#include "sources/gfgpu_perf_source.h"

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <iostream>

using Grafips::GpuPerfSource;

typedef void (PFNGLGETFIRSTPERFQUERYID) (GLuint *queryId);
typedef void (PFNGLGETNEXTPERFQUERYID) (GLuint queryId,
                                        GLuint *nextQueryId);

GpuPerfSource::GpuPerfSource(MetricSinkInterface *sink)
    : m_sink(sink) {

  const GLubyte* name =
      reinterpret_cast<const GLubyte*>("glGetFirstPerfQueryIdINTEL");

  std::vector<unsigned int> query_ids;

  void (*proc_addr)() = NULL;
  proc_addr = glXGetProcAddress(name);
  if (proc_addr == NULL)
    return;

  PFNGLGETFIRSTPERFQUERYID *p_glGetFirstPerfQueryIdINTEL =
      reinterpret_cast<PFNGLGETFIRSTPERFQUERYID*>(proc_addr);


  name = reinterpret_cast<const GLubyte*>("glGetNextPerfQueryIdINTEL");
  proc_addr = glXGetProcAddress(name);
  if (proc_addr == NULL)
    return;
  PFNGLGETNEXTPERFQUERYID *p_glGetNextPerfQueryIdINTEL = 
      reinterpret_cast<PFNGLGETNEXTPERFQUERYID*>(proc_addr);

  unsigned int query_id = 0;
  (*p_glGetFirstPerfQueryIdINTEL)(&query_id);

  if (query_id == 0)
    return;

  query_ids.push_back(query_id);
  while (true) {
    p_glGetNextPerfQueryIdINTEL(query_id, &query_id);
    if (! query_id )
      break;
    query_ids.push_back(query_id);
  }
}

GpuPerfSource::~GpuPerfSource() {
}

void
GpuPerfSource::GetDescriptions(MetricDescriptionSet *descriptions) {
}

void
GpuPerfSource::Enable(int id) {
}

void
GpuPerfSource::Disable(int id) {
}

void
GpuPerfSource::glSwapBuffers() {
}

