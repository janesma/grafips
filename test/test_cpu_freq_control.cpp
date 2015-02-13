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

#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>

#include <string>
#include <vector>

#include "controls/gfcontrol.h"
#include "controls/gfcontrol_stub.h"
#include "controls/gfcpu_freq_control.h"

using Grafips::CpuFreqControl;
using Grafips::FreqSysParser;
using Grafips::ControlSubscriberInterface;
using Grafips::ControlRouterTarget;
using Grafips::ControlRouterHost;
using Grafips::ControlSkel;
using Grafips::ControlStub;

class PolicySubscriber : public ControlSubscriberInterface {
 public:
  void OnControlChanged(const std::string &key,
                        const std::string &value) {
    ASSERT_EQ(key, "CpuFrequencyPolicy");
    policy = value;
  }
  std::string policy;
};


TEST(CpuFreqControl, test_instantiate ) {
  {
    FreqSysParser s;
    const int max_freq = atoi(s.MaxFreq().c_str());
    const int min_freq = atoi(s.MinFreq().c_str());
    if (max_freq != 0)
      EXPECT_GT(max_freq, min_freq);
  }

  std::string orig_policy;
  PolicySubscriber policy;
  {
    CpuFreqControl control;
    control.Subscribe(&policy);

    // non-root, can't get governor, so no publication of current state
    if (policy.policy =="") {
      EXPECT_FALSE(control.IsValid());
      return;
    }

    if ((policy.policy != "performance") &&
        (policy.policy != "powersave"))
      EXPECT_TRUE(false);
    orig_policy = policy.policy;

    control.Set("CpuFrequencyPolicy", "max_freq");
    FreqSysParser s;
    EXPECT_EQ(s.MaxFreq(), s.MinFreq());
    EXPECT_EQ(policy.policy, "max_freq");
    // destructor resets config to default
  }
  FreqSysParser s;
  EXPECT_NE(s.MaxFreq(), s.MinFreq());
  {
    CpuFreqControl control;
    control.Subscribe(&policy);
    // policy should be back to initial state
    EXPECT_EQ(orig_policy, policy.policy);
  }
}

TEST(CpuFreqControl, test_remote ) {
  PolicySubscriber policy;
  CpuFreqControl control;
  if (!control.IsValid())
    return;
  ControlRouterTarget target;
  ControlSkel skel(0, &target);
  skel.Start();
  {
    ControlRouterHost host("localhost", skel.GetPort());
    host.Subscribe("CpuFrequencyPolicy", &policy);
    host.Flush();

    target.AddControl("CpuFrequencyPolicy", &control);
    skel.Flush();
    EXPECT_TRUE(policy.policy == "performance" ||
                policy.policy == "powersave");

    host.Set("CpuFrequencyPolicy", "max_freq");
    host.Flush();
    skel.Flush();
    EXPECT_EQ(policy.policy, "max_freq");

    // destructor tears down the sockets
  }
  skel.Join();
}
