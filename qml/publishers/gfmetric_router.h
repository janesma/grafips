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

#ifndef QML_PUBLISHERS_GFMETRIC_ROUTER_H_
#define QML_PUBLISHERS_GFMETRIC_ROUTER_H_

#include <QObject>
#include <QString>
#include <QList>

#include <map>
#include <vector>

#include "remote/gfipublisher.h"
#include "remote/gfisubscriber.h"

namespace Grafips {
class GraphSetSubscriber;

class MetricRouter : public QObject,
                     public PublisherInterface,
                     public SubscriberInterface,
                     NoCopy, NoAssign, NoMove {
  Q_OBJECT
 public:
  MetricRouter();
  ~MetricRouter();
  // PublisherInterface
  void Enable(int id);
  void Disable(int id);
  void GetDescriptions(std::vector<MetricDescription> *descriptions) const;
  void Subscribe(SubscriberInterface *s);

  // SubscriberInterface
  void Clear(int id);
  void OnMetric(const DataSet &d);
  void OnDescriptions(const std::vector<MetricDescription> &descriptions);

  Q_INVOKABLE void AddGraph(GraphSetSubscriber* g, QList<QString> filters);

 private:
  std::map<int, GraphSetSubscriber*> m_routes;
};
}  // namespace Grafips

#endif  // QML_PUBLISHERS_GFMETRIC_ROUTER_H_
