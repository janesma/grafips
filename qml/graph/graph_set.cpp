#include "graph_set.h"

#include <float.h>

#include "metric.h"

GraphSet::GraphSet() : m_current_time(0), 
                       m_max_data_age(60 * 1000) 
{
}

void 
GraphSet::Add(const DataPoint &d)
{
    std::lock_guard<std::mutex> l(m_protect);
    if (d.time_val > m_current_time)
    {
        // new max time.  Drop any data which is too old to be displayed.
        m_current_time = d.time_val;
        while (!m_data.empty() &&
               m_data.begin()->first < m_current_time - m_max_data_age)
            m_data.erase(m_data.begin());
    }
    m_data[d.time_val] = d.data;
}

void
GraphSet::SetLimit(int max_data_age)
{
    std::lock_guard<std::mutex> l(m_protect);
    m_max_data_age = max_data_age;
}


void 
GraphSet::GetData(PointVec *data)
{
    std::lock_guard<std::mutex> l(m_protect);
    // TODO.  perhaps a point for every few pixels, with a LOD calculation
    data->resize(m_data.size());

    float max = FLT_MIN, min = FLT_MAX;
    
    for (std::map<int, float>::const_iterator i = m_data.begin(); i != m_data.end(); ++i)
    {
        if (i->second < min)
            min = i->second;
        if (i->second > max)
            max = i->second;
    }

    std::vector<Point>::iterator dest = data->begin(); 
    for (std::map<int, float>::const_iterator i = m_data.begin();
         i != m_data.end(); ++i, ++dest)
    {
        const float age = m_current_time - i->first;
        const float age_scaled = (2.0 * age / (float) m_max_data_age) - 1.0;
        dest->x = -1.0 * age_scaled;
        const float val_range = max - min;
        const float val_scaled = (2.0 * (i->second - min) / val_range) - 1.0;

        // for now, don't scale
        //dest->y = val_scaled;
        dest->y = -1.0 * (2.0 * i->second - 1.0);
    }
}

