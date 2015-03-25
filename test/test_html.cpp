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

#include "remote/gfmetric.h"
#include "publishers/gfhtml_output.h"

using Grafips::MetricDescriptionSet;
using Grafips::MetricDescription;
using Grafips::DataPoint;
using Grafips::DataSet;
using Grafips::HtmlOutput;
using Grafips::GR_METRIC_COUNT;
class BogoMet {
 public:
  BogoMet() {
    metrics.push_back(MetricDescription("one", "", "one", GR_METRIC_COUNT));
    metrics.push_back(MetricDescription("two", "", "two", GR_METRIC_COUNT));
    data.push_back(DataPoint(0, metrics[0].id(), 1.0));
    data.push_back(DataPoint(0, metrics[1].id(), 2.0));
    data.push_back(DataPoint(100, metrics[1].id(), 9.0));
    data.push_back(DataPoint(400, metrics[0].id(), 3.0));
    data.push_back(DataPoint(401, metrics[1].id(), 4.0));
  }
  MetricDescriptionSet metrics;
  DataSet data;
};

TEST(HtmlOutput, instantiate) {
  BogoMet b;
  {
    HtmlOutput h("/tmp/grafips_test.html");
    h.OnDescriptions(b.metrics);
    h.OnMetric(b.data);
    h.Activate(b.metrics[0].id());
    h.OnMetric(b.data);
    h.Activate(b.metrics[1].id());
    h.OnMetric(b.data);
    DataSet data;
    data.push_back(DataPoint(402, b.metrics[0].id(), 5.0));
    h.OnMetric(data);
    data[0] = DataPoint(403, b.metrics[1].id(), 6.0);
    h.OnMetric(data);

    data[0] = DataPoint(602, b.metrics[0].id(), 7.0);
    h.OnMetric(data);
    data[0] = DataPoint(603, b.metrics[1].id(), 8.0);
    h.OnMetric(data);

    data[0] = DataPoint(803, b.metrics[1].id(), 9.0);
    h.OnMetric(data);
}
}
