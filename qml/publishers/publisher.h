#pragma once

#include <vector>
#include <map>

#include <QObject> 

#include "metric.h"

class Provider;
class Subscriber;



// collates metrics from providers, distributes to subscriber (which may be
// off-proc or off-machine)
class Publisher : public QObject
{
    Q_OBJECT
  public:
    virtual ~Publisher() {}
    virtual void RegisterProvider(Provider *p) = 0;
    virtual void OnMetric(const DataSet &d) = 0;
    virtual void Enable(int id) = 0;
    virtual void Disable(int id) = 0;
    virtual void GetDescriptions(std::vector<MetricDescription> *descriptions) const = 0;
    virtual void Subscribe(Subscriber *) = 0;
};


class PublisherImpl : public Publisher
{
    Q_OBJECT

  public:
    Q_INVOKABLE void Subscribe(Subscriber *);

    PublisherImpl();
    ~PublisherImpl();
    void RegisterProvider(Provider *p);
    void OnMetric(const DataSet &d);
    Q_INVOKABLE void Enable(int id);
    Q_INVOKABLE void Disable(int id);
    void GetDescriptions(std::vector<MetricDescription> *descriptions) const;
  private:
    Subscriber *m_subscriber;
    typedef std::map <int, Provider*> ProviderMap;
    ProviderMap m_providersByMetricId;
    std::vector<Provider *> m_providers;
};

// handles off-machine publication
class PublisherProxy
{
};
