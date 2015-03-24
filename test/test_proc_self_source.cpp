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
#include <unistd.h>

#include "sources/gfproc_self_source.h"
#include "test/test_mock.h"
#include "remote/gfmetric.h"

using Grafips::ProcSelfSource;
using Grafips::TestPublisher;
using Grafips::get_ms_time;

TEST(ProcSelfSource, test_instantiate ) {
  ProcSelfSource source;
}

TEST(ProcSelfSource, test_subscribe ) {
  TestPublisher pub;
  ProcSelfSource source;
  source.Subscribe(&pub);
  EXPECT_EQ(pub.m_desc.size(), 2);
}

TEST(ProcSelfSource, test_rss ) {
  TestPublisher pub;
  ProcSelfSource source;
  source.Subscribe(&pub);

  int id;
  for (auto i = pub.m_desc.begin(); i != pub.m_desc.end(); ++i) {
    if (i->display_name == "Memory RSS") {
      id = i->id();
      source.Activate(id);
    }
  }

  // 300ms latency
  usleep(400 * 1000);
  source.Poll();
  EXPECT_GT(pub.m_d.size(), 0);
  EXPECT_EQ(pub.m_d[0].id, id);
  EXPECT_GT(pub.m_d[0].data, 0.0);
}

TEST(ProcSelfSource, test_utilization ) {
  TestPublisher pub;
  ProcSelfSource source;
  source.Subscribe(&pub);

  int id;
  for (auto i = pub.m_desc.begin(); i != pub.m_desc.end(); ++i) {
    if (i->display_name == "Process %CPU") {
      id = i->id();
      source.Activate(id);
    }
  }

  // 300ms latency
  const int start_time = get_ms_time();

  float v = 3.14159;
  while (get_ms_time() - start_time < 400) {
    v *= v;
  }
  source.Poll();
  EXPECT_GT(pub.m_d.size(), 0);
  EXPECT_EQ(pub.m_d[0].id, id);
  EXPECT_GT(pub.m_d[0].data, 90.0);
}
