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

#include <gtest/gtest.h>

#include <stdio.h>
#include <stdlib.h>

#include <waffle-1/waffle.h>

#include "sources/gfgpu_perf_source.h"

using Grafips::GpuPerfSource;

TEST(gpu_source, instantiate) {

  const int32_t init_attrs[] = {
    WAFFLE_PLATFORM, WAFFLE_PLATFORM_GBM,
    0,
  };
  waffle_init(init_attrs);

  struct waffle_display *dpy = waffle_display_connect(NULL);   
  const int32_t config_attrs[] = {
    WAFFLE_CONTEXT_API, WAFFLE_CONTEXT_OPENGL,
    0,
  };

  struct waffle_config *config = waffle_config_choose(dpy, config_attrs);
  struct waffle_context *ctx = waffle_context_create(config, NULL);
  struct waffle_window *window = waffle_window_create(config, 320, 320);
  waffle_make_current(dpy, window, ctx);

  GpuPerfSource s(NULL);
}
