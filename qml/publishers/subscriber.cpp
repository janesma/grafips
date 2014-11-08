#include "subscriber.h"

#include <assert.h>
#include <mutex>
#include "publisher.h"
#include "graph_set.h"

void 
GraphSetSubscriber::AddSet(int id, GraphSet *s)
{
    std::lock_guard<std::mutex> l(m_protect);
    m_dataSets[id] = s;
}

void 
GraphSetSubscriber::RemoveSet(int id)
{
    std::lock_guard<std::mutex> l(m_protect);
    m_dataSets[id] = NULL;
}

void
GraphSetSubscriber::OnMetric(const DataSet &d)
{
    std::lock_guard<std::mutex>l(m_protect);
    for (DataSet::const_iterator i = d.begin(); i != d.end(); ++i)
        if (NULL != m_dataSets[i->id])
            m_dataSets[i->id]->Add(*i);
}


void 
GraphSetSubscriber::OnDescriptions(const std::vector<MetricDescription> &descriptions)
{
    {
        std::lock_guard<std::mutex>l(m_protect);
        for (std::vector<MetricDescription>::const_iterator i = descriptions.begin(); i != descriptions.end(); ++i)
        {
            m_metrics.append(new QMetric(*i));
        }
    } 
    emit onEnabled();
}

QQmlListProperty<QMetric> 
GraphSetSubscriber::metrics()
{
    std::lock_guard<std::mutex>l(m_protect);
    return QQmlListProperty<QMetric>(this, m_metrics);
}

void
GraphSetSubscriber::Clear(int id)
{
    m_dataSets[id]->Clear();
}

GraphSetSubscriber::~GraphSetSubscriber()
{
}
