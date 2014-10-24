#include "publisher.h"
#include "provider.h"
#include "subscriber.h"

PublisherImpl::PublisherImpl()
{}

PublisherImpl::~PublisherImpl() 
{
}

void 
PublisherImpl::RegisterProvider(Provider *p)
{
    std::vector<MetricDescription> desc;
    p->GetDescriptions(&desc);
    for (int i = 0; i < desc.size(); ++i)
    {
        m_providersByMetricId[desc[i].id()] = p;
    }
}

void 
PublisherImpl::OnMetric(const DataSet &d)
{
    m_subscriber->OnMetric(d);
}

void 
PublisherImpl::Enable(int id)
{
    m_providersByMetricId[id]->Enable(id);
}

void 
PublisherImpl::GetDescriptions(std::vector<MetricDescription> *descriptions)
{
    std::vector<MetricDescription> *ret;
    for (ProviderMap::iterator i = m_providersByMetricId.begin(); i != m_providersByMetricId.end(); ++i)
        i->second->GetDescriptions(ret);
}

void 
PublisherImpl::Subscribe(Subscriber *s)
{
    m_subscriber = s;
}
