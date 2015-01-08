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
#include <QString>

#include <vector>

#include "sources/gfgl_source.h"
#include "grafips/test/test_mock.h"


using Grafips::TestPublisher;
using Grafips::GlSource;
using Grafips::MetricDescriptionSet;

TEST(GlSourceFixture, test_descriptions ) {
  TestPublisher pub;
  GlSource source;
  pub.RegisterSource(&source);

  source.Enable(pub.m_desc[0].id());

  source.glSwapBuffers();
  EXPECT_TRUE(pub.m_d.empty());

  usleep(100000);
  source.glSwapBuffers();
  EXPECT_LT(pub.m_d[0].data, 12.0);
  EXPECT_GT(pub.m_d[0].data, 9.6);

  pub.m_d.clear();

  source.Disable(pub.m_desc[0].id());
  source.Enable(pub.m_desc[1].id());

  usleep(100000);
  source.glSwapBuffers();
  EXPECT_LT(pub.m_d[0].data, 120.0);
  EXPECT_GT(pub.m_d[0].data, 90.6);
}
