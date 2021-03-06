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

#ifndef PUBLISHERS_GFHTML_OUTPUT_H_
#define PUBLISHERS_GFHTML_OUTPUT_H_

#include <string>
#include <vector>

#include "os/gfmutex.h"
#include "remote/gfmetric.h"

namespace Grafips {

class TableWriter;
class DataHeap;
class HtmlOutput {
 public:
  explicit HtmlOutput(const std::string &file_path);
  ~HtmlOutput();
  void OnMetric(const DataSet &d);
  void OnDescriptions(const MetricDescriptionSet &descriptions);
  void Activate(int id);
  void Deactivate(int id);
 private:
  DataHeap *m_heap;
  int m_fh;
  TableWriter *m_tw;
  std::vector<int> m_active_ids;
  MetricDescriptionSet m_desc;
  mutable Mutex m_protect;
};

}  // namespace Grafips

#endif  // PUBLISHERS_GFHTML_OUTPUT_H_
