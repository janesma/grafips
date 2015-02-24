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
#include <string>

#include "error/gferror.h"

using Grafips::Error;
using Grafips::ErrorHandler;
using Grafips::ErrorInterface;
using Grafips::ErrorTypes;
using Grafips::NoError;
using Grafips::Raise;
using Grafips::WARN;
using Grafips::kSocketWriteFail;

class TestHandler : public ErrorHandler {
 public:
  TestHandler() {}
  bool OnError(const ErrorInterface &e) {
    if (e.Type() == kSocketWriteFail) {
      m_msg = e.ToString();
      return true;
    }
    return false;
  }
  std::string m_msg;
};

TEST(Error, basic_handling) {
  TestHandler h;
  Raise(Error(kSocketWriteFail, WARN, "this is a test string"));
  EXPECT_EQ(h.m_msg, "this is a test string");
}


TEST(Error, scoped_handling) {
  {
    EXPECT_TRUE(NoError());
    TestHandler h1;
    EXPECT_TRUE(NoError());
    {
      EXPECT_TRUE(NoError());
      TestHandler h2;
      EXPECT_TRUE(NoError());
      Raise(Error(kSocketWriteFail, WARN, "this is a test string"));
      EXPECT_EQ(h2.m_msg, "this is a test string");
      EXPECT_EQ(h1.m_msg, "");
      EXPECT_FALSE(NoError());
    }
    EXPECT_TRUE(NoError());
    Raise(Error(kSocketWriteFail, WARN, ""));
    Raise(Error(kSocketWriteFail, WARN, "test"));
    EXPECT_EQ(h1.m_msg, "test");
    EXPECT_FALSE(NoError());
  }
  EXPECT_TRUE(NoError());
}
