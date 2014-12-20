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

#include <unistd.h>

#include "sources/gfcpu_source.h"
#include "sources/gfgpu_perf_source.h"
#include "sources/gfgl_source.h"
#include "remote/gfpublisher_skel.h"
#include "remote/gfpublisher.h"
#include "test/test_gpu_context.h"

using Grafips::CpuSource;
using Grafips::GlSource;
using Grafips::GpuPerfSource;
using Grafips::PublisherImpl;
using Grafips::PublisherSkeleton;
using Grafips::Thread;

class PollThread : public Thread {
 public:
  PollThread(CpuSource *c, GlSource *g) : Thread("PollThread"),
                                          m_running(false),
                                          m_cpu(c),
                                          m_gl(g) {}
  void Run() {
    m_running = true;
    while (m_running) {
      m_cpu->Poll();
      m_gl->glSwapBuffers();
      usleep(1000000);
    }
  }
  void Stop() {
    m_running = false;
    Join();
  }
 private:
  bool m_running;
  CpuSource *m_cpu;
  GlSource *m_gl;
};

int main(int argc, const char **argv) {
  Grafips::MockContext c;
  CpuSource prov;
  PublisherImpl pub;
  GlSource glprov(&pub);
  GpuPerfSource gpuprov;

  // TODO(majanes) make reactive
  gpuprov.MakeContextCurrent();
  gpuprov.SetMetricSink(&pub);

  PollThread thread(&prov, &glprov);

  prov.SetMetricSink(&pub);
  int port = 53136;
  if (argc > 1) {
    port = atoi(argv[1]);
  }
  PublisherSkeleton skel(port, &pub);
  skel.Start();
  thread.Start();

  skel.Join();
  thread.Stop();

  return 0;
}
