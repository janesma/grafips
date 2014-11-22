#pragma once

#include <QObject>

#include "gfmetric.h"

namespace Grafips
{
    // handles publications, distributes metric data to associated GraphSet
    class SubscriberInterface : public QObject
    {
        Q_OBJECT
      public slots:
        virtual void NotifyDescriptions() = 0;
      public:
        virtual ~SubscriberInterface() {}
        virtual void Clear(int id) = 0;
        virtual void OnMetric(const DataSet &d) = 0;
        virtual void OnDescriptions(const std::vector<MetricDescription> &descriptions) = 0;
    };
}
