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

#include "controls/gfapi_control.h"
#include "controls/gfcontrol.h"
#include "controls/gfcontrol_stub.h"
#include "controls/gfcpu_freq_control.h"
#include "error/gferror.h"
#include "remote/gfpublisher.h"
#include "remote/gfpublisher_skel.h"
#include "sources/gfcpu_clock_source.h"
#include "sources/gfcpu_source.h"
#include "sources/gfgl_source.h"
#include "sources/gfgpu_perf_source.h"
#include "test/test_gpu_context.h"

using Grafips::ApiControl;
using Grafips::ControlRouterTarget;
using Grafips::ControlSkel;
using Grafips::CpuFreqControl;
using Grafips::CpuFreqControl;
using Grafips::CpuFreqSource;
using Grafips::CpuSource;
using Grafips::GlSource;
using Grafips::GpuPerfSource;
using Grafips::NoError;
using Grafips::PublisherImpl;
using Grafips::PublisherSkeleton;
using Grafips::Thread;

class PollThread : public Thread {
 public:
  PollThread(CpuSource *c,
             GlSource *g,
             CpuFreqSource *f) : Thread("PollThread"),
                                 m_running(false),
                                 m_cpu(c),
                                 m_gl(g),
                                 m_f(f) {}
  void Run() {
    m_running = true;
    while (m_running) {
      m_cpu->Poll();
      if (!NoError())
        break;
      m_gl->glSwapBuffers();
      if (!NoError())
        break;
      m_f->Poll();
      if (!NoError())
        break;
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
  CpuFreqSource *m_f;
};

int main(int argc, const char **argv) {
  CpuSource prov;
  PublisherImpl pub;
  GlSource glprov;
  GpuPerfSource gpuprov;
  CpuFreqSource freqprov;

  pub.RegisterSource(&glprov);
  pub.RegisterSource(&gpuprov);
  pub.RegisterSource(&prov);
  pub.RegisterSource(&freqprov);

  Grafips::MockContext c;

  gpuprov.MakeContextCurrent();

  PollThread thread(&prov, &glprov, &freqprov);

  int port = 53136;
  if (argc > 1) {
    port = atoi(argv[1]);
  }

  PublisherSkeleton skel(port, &pub);

  CpuFreqControl freq_control;
  ApiControl api_control;
  ControlRouterTarget target;
  target.AddControl("CpuFrequencyPolicy", &freq_control);
  target.AddControl("ScissorExperiment", &api_control);
  ControlSkel control_skel(port + 1, &target);
  control_skel.Start();

  skel.Start();
  thread.Start();

  skel.Join();
  control_skel.Join();
  thread.Stop();

  return 0;
}
