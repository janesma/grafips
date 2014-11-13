#pragma once

#include <vector>
#include <map>

#include <QObject> 

#include "metric.h"
#include "gfipublisher.h"
#include "subscriber.h"

namespace Grafips
{
    class SubscriberInterface;
    class PublisherImpl : public PublisherInterface
    {
        Q_OBJECT

      public:
        Q_INVOKABLE void Subscribe(SubscriberInterface *);

        PublisherImpl();
        ~PublisherImpl();
        void RegisterProvider(Provider *p);
        void OnMetric(const DataSet &d);
        Q_INVOKABLE void Enable(int id);
        Q_INVOKABLE void Disable(int id);
        void GetDescriptions(std::vector<MetricDescription> *descriptions) const;
      private:
        SubscriberInterface *m_subscriber;
        typedef std::map <int, Provider*> ProviderMap;
        ProviderMap m_providersByMetricId;
        std::vector<Provider *> m_providers;
    };

// handles off-machine publication
    class PublisherProxy
    {
    };
}
