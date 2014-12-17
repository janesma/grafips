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
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/Xos.h>
//#include <X11/Xatom.h>
//#include <X11/keysym.h>
#include <GL/glx.h>

#include "sources/gfgpu_perf_source.h"

using Grafips::GpuPerfSource;

TEST(gpu_source, instantiate) {
    Display *dis = XOpenDisplay(NULL);
    Window win = XCreateSimpleWindow(dis, RootWindow(dis, 0), 1, 1, 500, 500,  \
                              0, BlackPixel (dis, 0), BlackPixel(dis, 0));
    XMapWindow(dis, win);
    XFlush(dis);

    int doubleBufferAttributes[] = {
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE,   GLX_RGBA_BIT,
        GLX_DOUBLEBUFFER,  True,  /* Request a double-buffered color buffer with */
        GLX_RED_SIZE,      1,     /* the maximum number of bits per component    */
        GLX_GREEN_SIZE,    1, 
        GLX_BLUE_SIZE,     1,
        None
    };

    int numReturned;
    GLXFBConfig          *fbConfigs = glXChooseFBConfig( dis, DefaultScreen(dis),
                                                         doubleBufferAttributes, &numReturned );
    assert(fbConfigs != NULL);

    GLXContext context = glXCreateNewContext(dis,  fbConfigs[0], GLX_RGBA_TYPE,
                                             NULL, True );

    glXMakeContextCurrent(dis, win, win, context);
    GpuPerfSource s(NULL);
}
