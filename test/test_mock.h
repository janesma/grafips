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

#ifndef TEST_TEST_MOCK_H__
#define TEST_TEST_MOCK_H__

#include "remote/gfipublisher.h"
#include "remote/gfimetric_sink.h"
#include "remote/gfisubscriber.h"
#include "sources/gfimetric_source.h"
#include "remote/gfmetric.h"

namespace Grafips {

class TestPublisher : public PublisherInterface, public MetricSinkInterface {
 public:
  TestPublisher() : m_p(NULL), m_sub(NULL) {}
  void RegisterSource(MetricSourceInterface *p) { m_p = p; p->Subscribe(this); }
  void OnMetric(const DataSet &d) {m_d.insert(m_d.end(), d.begin(), d.end()); }
  void Enable(int id) { m_p->Enable(id); }
  void Disable(int id) { m_p->Disable(id); }
  void OnDescriptions(const std::vector<MetricDescription> &descriptions) {
    m_desc = descriptions;
    if (m_sub)
      m_sub->OnDescriptions(descriptions);
  }
  void Subscribe(SubscriberInterface *s) {
    m_sub->OnDescriptions(m_desc);
  }
  DataSet m_d;
  MetricSourceInterface *m_p;
  std::vector<MetricDescription> m_desc;
  SubscriberInterface *m_sub;
};

}  // namespace Grafips

#endif  // TEST_TEST_MOCK_H__
