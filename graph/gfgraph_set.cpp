// Copyright (C) Intel Corp.  2014.  All Rights Reserved.

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice (including the
// next paragraph) shall be included in all copies or substantial
// portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//  **********************************************************************/
//  * Authors:
//  *   Mark Janes <mark.a.janes@intel.com>
//  **********************************************************************/


#include "graph/gfgraph_set.h"

#include <assert.h>

#include <float.h>
#include <map>
#include <vector>

#include "remote/gfmetric.h"

using Grafips::GraphSet;

GraphSet::GraphSet() : m_max_data_age(60 * 1000),
                       m_time_correction(0) {
}

void
GraphSet::Add(const DataPoint &d) {
  ScopedLock l(&m_protect);
  const unsigned int current_time = d.time_val;
  const unsigned int max_age = current_time - (1.1 * m_max_data_age);
  // Drop any data which is too old to be displayed.
  while (!m_data.empty() &&
         (unsigned int)(m_data.begin()->first) < max_age) {
    std::map<unsigned int, float>::iterator front = m_data.begin();
    assert(m_range.find(front->second) != m_range.end());
    m_range.erase(m_range.find(front->second));
    m_data.erase(front);
  }

  m_data[d.time_val] = d.data;
  m_range.emplace(d.data);
}

void
GraphSet::SetLimit(int max_data_age) {
  ScopedLock l(&m_protect);
  m_max_data_age = max_data_age;
}

void
GraphSet::GetData(PointVec *data, unsigned int request_time_ms) {
  ScopedLock l(&m_protect);
  data->resize(m_data.size());
  if (m_data.empty())
    return;

  const unsigned int most_recent_data_time = m_data.rbegin()->first;
  if (m_time_correction == 0) {
    // calculate the delta between our data set and the current system time

    // most_recent_data_time + correction ~~ request_time_ms
    m_time_correction = request_time_ms - most_recent_data_time;
  }

  // TODO(majanes) perhaps a point for every few pixels, with a LOD
  // calculation

  float max = FLT_MIN, min = FLT_MAX;

  for (std::map<unsigned int, float>::const_iterator i = m_data.begin();
       i != m_data.end(); ++i) {
    if (i->second < min)
      min = i->second;
    if (i->second > max)
      max = i->second;
  }

  std::vector<Point>::iterator dest = data->begin();
  for (std::map<unsigned int, float>::const_iterator i = m_data.begin();
       i != m_data.end(); ++i, ++dest) {
    // x value is the age in ms
    dest->x = static_cast<int>(request_time_ms) -
              static_cast<int>(m_time_correction + i->first);

    dest->y = i->second;
  }
}

void
GraphSet::Clear() {
  ScopedLock l(&m_protect);
  m_data.clear();
}

float
GraphSet::GetMax() const {
  ScopedLock l(&m_protect);
  if (m_range.empty())
    return 0;
  return *(m_range.rbegin());
}
