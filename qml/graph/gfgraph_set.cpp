#include "gfgraph_set.h"

#include <float.h>

#include "gfmetric.h"

using namespace Grafips;

GraphSet::GraphSet() : m_max_data_age(60 * 1000),
                       m_time_correction(0)
{
}

void 
GraphSet::Add(const DataPoint &d)
{
    std::lock_guard<std::mutex> l(m_protect);
    const unsigned int current_time = d.time_val;

    // Drop any data which is too old to be displayed.
    while (!m_data.empty() &&
           (unsigned int)(m_data.begin()->first) < (current_time - (1.1 * m_max_data_age)))
        m_data.erase(m_data.begin());

    m_data[d.time_val] = d.data;
}

void
GraphSet::SetLimit(int max_data_age)
{
    std::lock_guard<std::mutex> l(m_protect);
    m_max_data_age = max_data_age;
}


void 
GraphSet::GetData(PointVec *data, unsigned int request_time_ms)
{
    std::lock_guard<std::mutex> l(m_protect);
    data->resize(m_data.size());
    if (m_data.empty())
        return;

    const unsigned int most_recent_data_time = m_data.rbegin()->first; 
    if (m_time_correction == 0)
    {
        // calculate the delta between our data set and the current system time

        // most_recent_data_time + correction ~~ request_time_ms
        m_time_correction = request_time_ms - most_recent_data_time;
    }
    
    // TODO.  perhaps a point for every few pixels, with a LOD calculation

    float max = FLT_MIN, min = FLT_MAX;
    
    for (std::map<unsigned int, float>::const_iterator i = m_data.begin(); i != m_data.end(); ++i)
    {
        if (i->second < min)
            min = i->second;
        if (i->second > max)
            max = i->second;
    }

    std::vector<Point>::iterator dest = data->begin(); 
    for (std::map<unsigned int, float>::const_iterator i = m_data.begin();
         i != m_data.end(); ++i, ++dest)
    {
        // cast to int to prevent underflow, if data is newer than request
        const float age = (int) request_time_ms - (int)(m_time_correction + i->first);
        const float age_scaled = (2.0 * age / (float) m_max_data_age) - 1.0;
        dest->x = -1.0 * age_scaled;

        // for now, don't scale
        //const float val_range = max - min;
        //const float val_scaled = (2.0 * (i->second - min) / val_range) - 1.0;
        //dest->y = val_scaled;

        dest->y = -1.0 * (2.0 * i->second - 1.0);
    }
}

void
GraphSet::Clear()
{
    std::lock_guard<std::mutex> l(m_protect);
    m_data.clear();
}
