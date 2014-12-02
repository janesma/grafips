#pragma once

#include <map>
#include <vector>
#include <mutex>

#include <QObject>
#include <QString> 
#include <QQmlListProperty>

#include "gfmetric.h"
#include "gfisubscriber.h"
#include "os/gftraits.h"

namespace Grafips
{

    class GraphSet;

    class QMetric : public QObject,
                    NoCopy, NoAssign, NoMove
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ name NOTIFY onName)
        Q_PROPERTY(int met_id READ met_id NOTIFY onMet_id)
        Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY onEnabled)
      public:
        QMetric() : m_id(-1), m_name(), m_enabled(false) {}

        explicit QMetric(const MetricDescription &m) : m_id(m.id()), 
                                                       m_name(QString::fromStdString(m.display_name)), 
                                                       m_enabled(false) {}
        QString name() { return m_name; }
        int met_id() { return m_id; }
        bool enabled() { return m_enabled; }
        void setEnabled(bool e) 
            { 
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
                               NoCopy, NoAssign, NoMove
    {
        Q_OBJECT
        Q_PROPERTY(QQmlListProperty<Grafips::QMetric> metrics READ metrics NOTIFY onEnabled)

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
        mutable std::mutex m_protect;
    };
}
