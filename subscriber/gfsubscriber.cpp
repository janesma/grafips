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

#include "subscriber/gfsubscriber.h"

#include <assert.h>
#include <vector>

#include "remote/gfpublisher.h"
#include "graph/gfgraph_set.h"

using Grafips::GraphSetSubscriber;

void
GraphSetSubscriber::AddSet(int id, GraphSet *s) {
  ScopedLock l(&m_protect);
  m_dataSets[id] = s;
}

void
GraphSetSubscriber::RemoveSet(int id) {
  ScopedLock l(&m_protect);
  m_dataSets[id] = NULL;
}

void
GraphSetSubscriber::OnMetric(const DataSet &d) {
  ScopedLock l(&m_protect);
  for (DataSet::const_iterator i = d.begin(); i != d.end(); ++i)
    if (NULL != m_dataSets[i->id]) {
      const DataPoint &p = *i;
      m_dataSets[p.id]->Add(p);
    }
}



void
GraphSetSubscriber::Clear(int id) {
    if (m_dataSets.find(id) == m_dataSets.end())
        return;
    m_dataSets[id]->Clear();
}

GraphSetSubscriber::GraphSetSubscriber() {
}

GraphSetSubscriber::~GraphSetSubscriber() {
}

void
GraphSetSubscriber::GetIDs(std::vector<int> *ids) const {
  ScopedLock l(&m_protect);
  for (std::vector<MetricDescription>::const_iterator i
           = m_descriptions.begin();
       i != m_descriptions.end(); ++i) {
    ids->push_back(i->id());
  }
}

void
GraphSetSubscriber::OnDescriptions(const MetricDescriptionSet &descriptions) {
  ScopedLock l(&m_protect);
  m_descriptions = descriptions;
}
