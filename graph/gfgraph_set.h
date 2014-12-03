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

#ifndef GRAPH_GFGRAPH_SET_H_
#define GRAPH_GFGRAPH_SET_H_

#include <GLES2/gl2.h>

#include <map>
#include <vector>

#include "os/gftraits.h"
#include "os/gfmutex.h"

namespace Grafips {

class DataPoint;

// interpolates data, provides it to view
class GraphSet : NoCopy, NoAssign, NoMove {
 public:
  struct Point {
    GLfloat x;
    GLfloat y;
  };
  typedef std::vector<Point> PointVec;

  GraphSet();
  void Add(const DataPoint &d);
  void SetLimit(int max_data_age);
  void SetWidth(int w) { m_width = w; }
  void GetData(PointVec *data, unsigned int request_time_ms);
  void Clear();

 private:
  Mutex m_protect;
  std::map<unsigned int, float> m_data;
  int m_max_data_age, m_width, m_time_correction;
};

}  // namespace Grafips

#endif  // GRAPH_GFGRAPH_SET_H_
