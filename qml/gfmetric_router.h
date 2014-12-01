#pragma once

namespace Grafips
{
    class MetricRouter : public PublisherInterface, public SubscriberInterface
    {
        // PublisherInterface
        void RegisterSource(MetricSourceInterface *p);
        void OnMetric(const DataSet &d);
        void Enable(int id);
        void Disable(int id);
        void GetDescriptions(std::vector<MetricDescription> *descriptions) const;
        void Subscribe(SubscriberInterface *);

        // SubscriberInterface
        void Clear(int id);
        void OnMetric(const DataSet &d);
        void OnDescriptions(const std::vector<MetricDescription> &descriptions);

    };
}
