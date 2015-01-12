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


#include "sources/gfgpu_perf_source.h"
#include "test/test_gpu_context.h"
#include "test/test_mock.h"

using Grafips::GpuPerfSource;
using Grafips::MetricDescriptionSet;
using Grafips::MockContext;
using Grafips::TestPublisher;

TEST(gpu_source, instantiate) {
  // instantiate the metrics source that queries the perf api
  GpuPerfSource s;
  TestPublisher p;
  p.RegisterSource(&s);

  MockContext m;

  EXPECT_EQ(p.m_desc.size(), 0);

  s.MakeContextCurrent();
  EXPECT_GT(p.m_desc.size(), 0);
}

TEST(gpu_source, metric_values) {

  GpuPerfSource s;
  TestPublisher p;
  p.RegisterSource(&s);

  MockContext m;
  s.MakeContextCurrent();
  
  EXPECT_GT(p.m_desc.size(), 0);

  int id = 0;
  const std::string path1 = "gpu/intel/2/EU Active";
  for (auto i = p.m_desc.begin(); i != p.m_desc.end(); ++i) {
    if (i->path == path1 ) {
      id = i->id();
      break;
    }
  }
  ASSERT_NE(id, 0);
  p.Enable(id);
  for (int i = 0; i < 10; ++i) {
    m.Draw();
    s.glSwapBuffers();
  }
  p.Disable(id);
    
  EXPECT_GT(p.m_d.size(), 0);
  p.m_d.clear();

  const std::string path = "gpu/intel/28/GPU Timestamp";
  id = 0;
  for (auto i = p.m_desc.begin(); i != p.m_desc.end(); ++i) {
    if (i->path == path ) {
      id = i->id();
      break;
    }
  }
  ASSERT_NE(id, 0);
  p.Enable(id);
  for (int i = 0; i < 10; ++i) {
    m.Draw();
    s.glSwapBuffers();
  }
  p.Disable(id);
    
  EXPECT_GT(p.m_d.size(), 0);
}
