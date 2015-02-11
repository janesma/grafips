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

#include "controls/gfcpu_freq_control.h"

using Grafips::CpuFreqControl;



class FreqState {
 public:
  FreqState() {
    static const int LINE_LEN = 100;
    std::vector<char> buf(LINE_LEN + 1);
    buf[LINE_LEN] = '\0';

    static const char *gov_path = "/sys/devices/system/cpu/cpu0/cpufreq/"
                                  "scaling_governor";
    int governor_fh = open(gov_path, O_RDONLY);
    ssize_t nbytes = read(governor_fh, buf.data(), LINE_LEN);
    assert(nbytes > 0);
    buf[nbytes] = '\0';
    governor = buf[0];

    static const char *max_path = "/sys/devices/system/cpu/cpu0/cpufreq/"
                                  "scaling_max_freq";
    int max_fh = open(max_path, O_RDONLY);
    assert(max_fh);
    nbytes = read(max_fh, buf.data(), LINE_LEN);
    assert(nbytes > 0);
    buf[nbytes] = '\0';
    max_freq = atoi(buf.data());

    static const char *min_path = "/sys/devices/system/cpu/cpu0/cpufreq/"
                                  "scaling_min_freq";
    int min_fh = open(min_path, O_RDONLY);
    assert(min_fh);
    nbytes = read(min_fh, buf.data(), LINE_LEN);
    assert(nbytes > 0);
    buf[nbytes] = '\0';

    min_freq = atoi(buf.data());
  }
  int max_freq;
  int min_freq;
  std::string governor;
};


TEST(CpuFreqControl, test_instantiate ) {
  {
    FreqState s;
    EXPECT_GT(s.max_freq, s.min_freq);
  }

  std::string policy;
  {
    CpuFreqControl control;

    // non-root, can't get governor
    if (!control.Get("CpuFrequencyPolicy", &policy)) {
      EXPECT_FALSE(control.IsValid());
      return;
    }

    if ((policy != "performance") && (policy != "powersave"))
      EXPECT_TRUE(false);

    control.Set("CpuFrequencyPolicy", "max_freq");
    FreqState s;
    EXPECT_EQ(s.max_freq, s.min_freq);

    // destructor resets config to default
  }
  FreqState s;
  EXPECT_NE(s.max_freq, s.min_freq);
  CpuFreqControl control;
  std::string new_policy;
  EXPECT_TRUE(control.Get("CpuFrequencyPolicy", &new_policy));
  EXPECT_EQ(new_policy, policy);
}
