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
#include "publishers/gfhtml_output.h"

using Grafips::MetricRouter;
using Grafips::QMetric;
using Grafips::DataSet;
using Grafips::MetricDescriptionSet;

MetricRouter::MetricRouter(): m_output(NULL) {
  connect(this, SIGNAL(NotifyDescriptions()),
          this, SLOT(HandleNotifyDescriptions()));
}

MetricRouter::~MetricRouter() {
  if (m_output)
    delete m_output;
}

void
MetricRouter::ActivateToGraph(int id, GraphSetSubscriber *dest) {
  ScopedLock l(&m_protect);

  // TODO(majanes): probably want to clear any GraphSetSubscriber that
  // is already enabled for the id.  Maybe do that first at a higher
  // level then assert here.

  m_routes[id] = dest;
  Activate(id);
}

void
MetricRouter::Activate(int id) {
  m_pub.Activate(id);
  if (m_output)
    m_output->Activate(id);
}

void
MetricRouter::Deactivate(int id) {
  ScopedLock l(&m_protect);
  m_pub.Deactivate(id);
  if (m_output)
    m_output->Deactivate(id);
}

void
MetricRouter::Subscribe(SubscriberInterface *s) {
  // GraphSetSubscriber do not call subscribe.  Instead, they AddGraph
  // for a specific set of filters.
  assert(false);
}

void
MetricRouter::Clear(int id) {
  ScopedLock l(&m_protect);
  m_routes[id]->Clear(id);
}

void
MetricRouter::OnMetric(const DataSet &d) {
  ScopedLock l(&m_protect);
  std::map<int, DataSet> demux;
  for (DataSet::const_iterator i = d.begin(); i != d.end(); ++i) {
    demux[i->id].push_back(*i);
  }
  for (std::map<int, DataSet>::const_iterator i = demux.begin();
       i != demux.end(); ++i) {
    m_routes[i->first]->OnMetric(i->second);
  }
  if (m_output)
    m_output->OnMetric(d);
}

void
MetricRouter::OnDescriptions(const MetricDescriptionSet &descriptions) {
  // cache the descriptions.  This call may occur before
  // GraphSetSubscriber objects are added for specific filters


  // ensure thread access is protected
  ScopedLock l(&m_protect);

  m_descriptions.clear();
  for (auto i = descriptions.begin(); i != descriptions.end(); ++i) {
    m_descriptions[i->path] = *i;
  }

  for (SubscriberList::const_iterator i = m_subscribers.begin();
       i != m_subscribers.end(); ++i) {
    (*i)->OnDescriptions(descriptions);
  }

  if (m_output)
    m_output->OnDescriptions(descriptions);

  // Pass a signal to self so that m_metrics property can be updated
  // in the UI thread.
  emit NotifyDescriptions();
}

void
MetricRouter::AddGraph(GraphSetSubscriber* g) {
  ScopedLock l(&m_protect);
  m_subscribers.push_back(g);
  MetricDescriptionSet desc(m_descriptions.size());
  for (auto i = m_descriptions.begin(); i != m_descriptions.end(); ++i) {
    desc.push_back(i->second);
  }
  g->OnDescriptions(desc);
}

void
MetricRouter::setAddress(const QString &a) {
  ScopedLock l(&m_protect);
  m_pub.setAddress(a);
  m_pub.Subscribe(this);
}



QQmlListProperty<QMetric>
MetricRouter::metrics() {
  ScopedLock l(&m_protect);
  return QQmlListProperty<QMetric>(this, m_metrics);
}


// occurs in the UI thread, converts metrics descriptions into
// Qlist<QMetrics> property
void
MetricRouter::HandleNotifyDescriptions() {
  {
    ScopedLock l(&m_protect);
    m_metrics.clear();
    for (auto i = m_descriptions.begin();
         i != m_descriptions.end(); ++i) {
      m_metrics.append(new QMetric(i->second));
    }
  }
  emit onEnabled();
}

void
MetricRouter::SetText(bool capture) {
  ScopedLock l(&m_protect);
  if (capture) {
    if (m_output) {
      delete m_output;
      m_output = NULL;
    }
    m_output = new HtmlOutput;
    return;
  }

  // else
  if (m_output) {
    delete m_output;
  }
  m_output = NULL;
}
