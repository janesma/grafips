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

#ifndef TEST_TEST_GPU_CONTEXT_H_
#define TEST_TEST_GPU_CONTEXT_H_

#include <waffle-1/waffle.h>

namespace Grafips {
class MockContext {
 public:
  MockContext() {
    const int32_t init_attrs[] = {
      WAFFLE_PLATFORM, WAFFLE_PLATFORM_GBM,
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
  }

  ~MockContext() {
    waffle_context_destroy(m_ctx);
    waffle_window_destroy(m_window);
    waffle_config_destroy(m_config);
    waffle_display_disconnect(m_dpy);
  }

 private:
  struct waffle_display *m_dpy;
  struct waffle_config *m_config;
  struct waffle_window *m_window;
  struct waffle_context *m_ctx;
};
}  // namespace Grafips
#endif  // TEST_TEST_GPU_CONTEXT_H_
