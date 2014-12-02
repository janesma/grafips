#pragma once

#include <QObject>
#include <QString> 
#include <QList>
#include <map>

#include "gfipublisher.h"
#include "gfisubscriber.h"

namespace Grafips
{
    class GraphSetSubscriber;
    
    class MetricRouter : public QObject,
                         public PublisherInterface,
                         public SubscriberInterface
    {
        Q_OBJECT
      public:
        MetricRouter() {}
        ~MetricRouter() {}
        // PublisherInterface
        void Enable(int id) {}
        void Disable(int id) {}
        void GetDescriptions(std::vector<MetricDescription> *descriptions) const {}
        void Subscribe(SubscriberInterface *) {}

        // SubscriberInterface
        void Clear(int id) {}
        void OnMetric(const DataSet &d) {}
        void OnDescriptions(const std::vector<MetricDescription> &descriptions) {}

        Q_INVOKABLE void AddGraph(GraphSetSubscriber* g, QList<QString> filters) {}

      private:
        std::map<int, GraphSetSubscriber*> m_routes;
    };
}
