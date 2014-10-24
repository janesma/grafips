#include "subscriber.h"
#include "publisher.h"
#include "graph_set.h"

void 
GraphSetSubscriber::AddSet(int id, GraphSet *s)
{
    m_dataSets[id] = s;
}

void
GraphSetSubscriber::OnMetric(const DataSet &d)
{
    for (DataSet::const_iterator i = d.begin(); i != d.end(); ++i)
        m_dataSets[i->id]->Add(*i);
}
