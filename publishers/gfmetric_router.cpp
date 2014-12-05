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

#include "publishers/gfmetric_router.h"

#include <vector>
#include <map>

#include "subscriber/gfsubscriber.h"
#include "publishers/gfpattern.h"

using Grafips::MetricRouter;

MetricRouter::MetricRouter() {}
MetricRouter::~MetricRouter() {}

void
MetricRouter::Enable(int id) {
  m_pub.Enable(id);
}

void
MetricRouter::Disable(int id) {
  m_pub.Disable(id);
}

void
MetricRouter::Subscribe(SubscriberInterface *s) {
  // GraphSetSubscriber do not call subscribe.  Instead, they AddGraph
  // for a specific set of filters.
  assert(false);
}

void
MetricRouter::Clear(int id) {
  m_routes[id]->Clear(id);
}

void
MetricRouter::OnMetric(const DataSet &d) {
  std::map<int, DataSet> demux;
  for (DataSet::const_iterator i = d.begin(); i != d.end(); ++i) {
    demux[i->id].push_back(*i);
  }
  for (std::map<int, DataSet>::const_iterator i = demux.begin();
       i != demux.end(); ++i) {
    m_routes[i->first]->OnMetric(i->second);
  }
}

void
MetricRouter::OnDescriptions(const MetricDescriptionSet &descriptions) {
  // cache the descriptions.  It is assumed that this call occurs
  // before GraphSetSubscriber objects are added for specific filters
  m_descriptions = descriptions;

  for (FilterMap::const_iterator i = m_filters.begin();
       i != m_filters.end(); ++i) {
    NotifyMetrics(i->first, i->second);
  }
}

void
MetricRouter::AddGraph(GraphSetSubscriber* g, QList<QString> filters) {
  m_filters[g] = filters;

  NotifyMetrics(g, filters);
}

void
MetricRouter::NotifyMetrics(GraphSetSubscriber* g,
                            const QList<QString> &filters) {
  // iterate the cached descriptions, and allocate them based on the
  // filters.  Then notify the GraphSetSubscriber as to which metrics
  // matched the filters.
  MetricDescriptionSet matching_metrics;
  PatternSet pattern(filters);
  for (MetricDescriptionSet::const_iterator i = m_descriptions.begin();
       i != m_descriptions.end(); ++i) {
    if (pattern.Matches(i->path)) {
      // only one graph may display a given metric
      assert(m_routes.find(i->id()) == m_routes.end() || m_routes[i->id()] == g);
      m_routes[i->id()] = g;

      matching_metrics.push_back(*i);
    }
  }
  if (!matching_metrics.empty())
    g->OnDescriptions(matching_metrics);
}

void
MetricRouter::setAddress(const QString &a) {
  m_pub.setAddress(a);
  m_pub.Subscribe(this);
}
