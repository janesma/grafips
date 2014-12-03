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

#ifndef SUBSCRIBER_GFSUBSCRIBER_H_
#define SUBSCRIBER_GFSUBSCRIBER_H_

#include <QObject>
#include <QString>
#include <QQmlListProperty>

#include <map>
#include <vector>

#include "remote/gfmetric.h"
#include "remote/gfisubscriber.h"
#include "os/gftraits.h"
#include "os/gfmutex.h"

namespace Grafips {

class GraphSet;

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

class GraphSetSubscriber : public QObject, public SubscriberInterface,
                           NoCopy, NoAssign, NoMove {
  Q_OBJECT
  Q_PROPERTY(QQmlListProperty<Grafips::QMetric> metrics
             READ metrics NOTIFY onEnabled)

 public slots:
  void HandleNotifyDescriptions();

 signals:
  void NotifyDescriptions();

 public:
  GraphSetSubscriber();
  ~GraphSetSubscriber();
  void AddSet(int id, GraphSet *);
  void RemoveSet(int id);
  void Clear(int id);
  void OnMetric(const DataSet &d);
  void OnDescriptions(const std::vector<MetricDescription> &descriptions);
  QQmlListProperty<QMetric> metrics();

  void GetIDs(std::vector<int> *ids) const;

 signals:
  void onEnabled();

 private:
  QList<QMetric *> m_metrics;
  std::vector<MetricDescription> m_metric_descriptions;
  std::map<int, GraphSet *> m_dataSets;
  mutable Mutex m_protect;
};
}  // namespace Grafips
#endif  // SUBSCRIBER_GFSUBSCRIBER_H_
