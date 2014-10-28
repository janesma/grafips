#include "subscriber.h"

#include <assert.h>

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


void 
GraphSetSubscriber::OnDescriptions(const std::vector<MetricDescription> &descriptions)
{
    for (std::vector<MetricDescription>::const_iterator i = descriptions.begin(); i != descriptions.end(); ++i)
    {
        m_metrics.append(new QMetric(*i));
    }
    //emit QAbstractListModel::dataChanged(createIndex(0,0), createIndex(m_metrics.size(),0));
}

QQmlListProperty<QMetric> 
GraphSetSubscriber::metrics()
{
    return QQmlListProperty<QMetric>(this, m_metrics);
}

// int 
// GraphSetSubscriber::rowCount(const QModelIndex &) const
// {
//     return m_metrics.size();
// }

// QVariant 
// GraphSetSubscriber::data(const QModelIndex & index, int role) const
// {
//     assert(index.row() < m_metrics.size());
//     std::map<QString, QMetric>::const_iterator i = m_metrics.begin();
//     int row = index.row();
//     while (row > 0)
//         ++i;
//     if (role == NameRole)
//         return i->first;
//     return i->second.enabled;
// }


// QVariant 
// GraphSetSubscriber::headerData(int section, Qt::Orientation orientation, int role) const
// {
//     return QString("Metric Name");
// }

