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
#include <QUrl>

#include <map>
#include <vector>
#include <string>

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
  Q_PROPERTY(QString helpText READ helpText NOTIFY onHelpText)
  Q_PROPERTY(int met_id READ met_id NOTIFY onMet_id)
  Q_PROPERTY(bool metEnabled READ metEnabled NOTIFY onMetEnabled)
 public:
  QMetric() : m_id(-1), m_name(), m_help_text(),
              m_enabled(true) {}

  explicit QMetric(const MetricDescription &m)
      : m_id(m.id()),
        m_name(QString::fromStdString(m.display_name)),
        m_help_text(QString::fromStdString(m.help_text)),
        m_enabled(m.enabled) {}
  QString name() { return m_name; }
  QString helpText() {return m_help_text;}
  int met_id() { return m_id; }
  bool metEnabled() {return m_enabled;}

 signals:
  void onMetEnabled();

  // these exist just to avoid run-time warnings.  names & descriptions do
  // not change
  void onName();
  void onHelpText();
  void onMet_id();

 private:
  QMetric(const QMetric&);
  QMetric& operator=(const QMetric&);
  int m_id;
  QString m_name, m_help_text;
  bool m_enabled;
};

class HtmlOutput;
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
  Q_INVOKABLE void ActivateToGraph(int id, GraphSetSubscriber *dest);
  // PublisherInterface
  void Activate(int id);
  Q_INVOKABLE void Deactivate(int id);
  void Subscribe(SubscriberInterface *s);

  // SubscriberInterface
  void Clear(int id);
  void OnMetric(const DataSet &d);
  void OnDescriptions(const std::vector<MetricDescription> &descriptions);

  Q_INVOKABLE void SetText(bool capture);
  Q_INVOKABLE void SetTextFile(QUrl filename);
  Q_INVOKABLE void AddGraph(GraphSetSubscriber* g);
  QString address() const { return m_pub.address(); }
  void setAddress(const QString &a);

  QQmlListProperty<QMetric> metrics();

  void OnDisconnect();

 signals:
  // sent to self, from the skeleton thread, to pass control to the UI
  // thread, where QMetric Qlist can be updated in QML.  Connected to
  // self's HandleNotifyDescriptions
  void NotifyDescriptions();
  void stop();

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
  // key is description path, to keep the metrics sorted
  std::map<std::string, MetricDescription> m_descriptions;

  // tracks active metrics, so that they can be printed in the case
  // where output is enabled during active metrics collection.
  std::map<int, bool> m_active_ids;
  HtmlOutput *m_output;
  QUrl m_fileName;
  mutable Mutex m_protect;
};
}  // namespace Grafips

#endif  // PUBLISHERS_GFMETRIC_ROUTER_H_
