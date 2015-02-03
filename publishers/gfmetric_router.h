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

#ifndef PUBLISHERS_GFMETRIC_ROUTER_H_
#define PUBLISHERS_GFMETRIC_ROUTER_H_

#include <QObject>
#include <QString>
#include <QList>

#include <map>
#include <vector>

#include "os/gftraits.h"
#include "remote/gfipublisher.h"
#include "remote/gfisubscriber.h"
#include "publishers/gfpublisher_remote.h"

namespace Grafips {
class GraphSetSubscriber;

class QMetric : public QObject,
                NoCopy, NoAssign, NoMove {
  Q_OBJECT
  Q_PROPERTY(QString name READ name NOTIFY onName)
  Q_PROPERTY(int met_id READ met_id NOTIFY onMet_id)
  Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY onEnabled)
 public:
  QMetric() : m_id(-1), m_name(), m_enabled(false) {}

  explicit QMetric(const MetricDescription &m)
      : m_id(m.id()),
        m_name(QString::fromStdString(m.display_name)),
        m_enabled(false) {}
  QString name() { return m_name; }
  int met_id() { return m_id; }
  bool enabled() { return m_enabled; }
  void setEnabled(bool e) {
    m_enabled = e;
    emit onEnabled();
  }
 signals:
  void onEnabled();

  // these exist just to avoid run-time warnings.  names & descriptions do
  // not change
  void onName();
  void onMet_id();
 private:
  QMetric(const QMetric&);
  QMetric& operator=(const QMetric&);
  int m_id;
  QString m_name;
  bool m_enabled;
};

class MetricRouter : public QObject,
                     public PublisherInterface,
                     public SubscriberInterface,
                     NoCopy, NoAssign, NoMove {
  Q_OBJECT
  Q_PROPERTY(QString address READ address WRITE setAddress)
  Q_PROPERTY(QQmlListProperty<Grafips::QMetric> metrics
             READ metrics NOTIFY onEnabled)
 public:
  MetricRouter();
  ~MetricRouter();
  Q_INVOKABLE void EnableToGraph(int id, GraphSetSubscriber *dest);
  // PublisherInterface
  void Enable(int id);
  Q_INVOKABLE void Disable(int id);
  void Subscribe(SubscriberInterface *s);

  // SubscriberInterface
  void Clear(int id);
  void OnMetric(const DataSet &d);
  void OnDescriptions(const std::vector<MetricDescription> &descriptions);

  Q_INVOKABLE void AddGraph(GraphSetSubscriber* g);
  QString address() const { return m_pub.address(); }
  void setAddress(const QString &a);

  QQmlListProperty<QMetric> metrics();

 signals:
  // sent to self, from the skeleton thread, to pass control to the UI
  // thread, where QMetric Qlist can be updated in QML.  Connected to
  // self's HandleNotifyDescriptions
  void NotifyDescriptions();

  // bound to QML in MetricList via the metrics property
  void onEnabled();

 public slots:
  // recives NotifyDescriptions() signal on the UI thread, then emits
  // onEnabled for MetricList.qml to recieve
  void HandleNotifyDescriptions();

 private:
  std::map<int, GraphSetSubscriber*> m_routes;
  QList<QMetric *> m_metrics;
  typedef std::vector<GraphSetSubscriber*> SubscriberList;
  SubscriberList m_subscribers;
  PublisherStub m_pub;
  MetricDescriptionSet m_descriptions;
  mutable Mutex m_protect;
};
}  // namespace Grafips

#endif  // PUBLISHERS_GFMETRIC_ROUTER_H_
