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

#include "publishers/gfhtml_output.h"

#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "error/gflog.h"

using Grafips::DataPoint;
using Grafips::DataSet;
using Grafips::HtmlOutput;
using Grafips::MetricDescriptionSet;
using Grafips::Mutex;
using Grafips::TableWriter;

class TimeCompare {
 public:
  bool operator()(const DataPoint &a, const DataPoint &b) {
    return a.time_val > b.time_val;
  }
};

class IdCompare{
 public:
  bool operator()(const DataPoint &a, const DataPoint &b) {
    return a.id < b.id;
  }
};

// orders data by time and then by id
class Grafips::DataHeap {
 public:
  void Push(const DataSet &d) {
    for (auto i = d.begin(); i != d.end(); ++i) {
      m_heap.push(*i);
    }
  }
  // output DataSet ordered by id.  id occurs at most one time.
  void Pop(DataSet *d, unsigned int current_time) {
    if (m_heap.empty())
      return;

    // tracks ids, so there is at most one value per column
    std::set<int> ids;

    while (true) {
      if (m_heap.empty())
        break;
      const DataPoint &top = m_heap.top();
      if (current_time < top.time_val + 100)
        break;

      if (ids.find(top.id) != ids.end())
        break;

      ids.insert(top.id);
      d->push_back(top);
      m_heap.pop();
    }

    // metrics must be in the same order as the headers (sorted by id)
    std::sort(d->begin(), d->end(), IdCompare());
  }
  void Clear() {
    while (!m_heap.empty())
      m_heap.pop();
  }

 private:
  std::priority_queue<DataPoint, DataSet, TimeCompare> m_heap;
};

class TableWriter {
 public:
  TableWriter(const MetricDescriptionSet &m,
              const std::vector<int> &active_ids,
              int fh);
  ~TableWriter();
  void Write(const DataSet &d);
 private:
  std::vector<int> m_ids;
  std::vector<char> m_buf;
  int m_fh;
};

HtmlOutput::HtmlOutput(const std::string &file_path)
    : m_heap(new DataHeap),
      m_tw(NULL) {
  m_fh = open(file_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
              S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  static const char *BEGIN_FILE = "<html>\n<body>\n";
  write(m_fh, BEGIN_FILE, strlen(BEGIN_FILE));
}

HtmlOutput::~HtmlOutput() {
  if (m_tw)
    delete m_tw;
  static const char *END_FILE = "</body>\n</html>\n";
  write(m_fh, END_FILE, strlen(END_FILE));
  close(m_fh);
  delete m_heap;
}

void
HtmlOutput::OnMetric(const DataSet &d) {
  if (!m_tw)
    return;
  if (d.empty())
    return;
  ScopedLock l(&m_protect);
  m_heap->Push(d);
  while (true) {
    DataSet ready;
    m_heap->Pop(&ready, d.back().time_val);
    if (ready.empty())
      break;
    m_tw->Write(ready);
  }
}

void
HtmlOutput::OnDescriptions(const MetricDescriptionSet &descriptions) {
  ScopedLock l(&m_protect);
  m_desc = descriptions;
}

void
HtmlOutput::Activate(int id) {
  ScopedLock l(&m_protect);
  m_active_ids.push_back(id);
  std::sort(m_active_ids.begin(), m_active_ids.end());
  m_heap->Clear();
  delete m_tw;
  m_tw = new TableWriter(m_desc, m_active_ids, m_fh);
}

void
HtmlOutput::Deactivate(int id) {
  ScopedLock l(&m_protect);
  for (auto i = m_active_ids.begin(); i != m_active_ids.end(); ++i) {
    if (*i == id) {
      *i = m_active_ids.back();
      m_active_ids.pop_back();
      break;
    }
  }
  m_heap->Clear();
  std::sort(m_active_ids.begin(), m_active_ids.end());
  delete m_tw;
  m_tw = new TableWriter(m_desc, m_active_ids, m_fh);
}


static const char *BEGIN_TABLE = "<table>\n";
static const char *END_TABLE = "</table>\n";
static const char *TR = "<tr>";
static const char *TRE = "</tr>\n";
static const char *TD = "<td>";
static const char *TDE = "</td>";

void write_field(int fh, const char *field) {
  write(fh, field, strlen(field));
}

TableWriter::TableWriter(const MetricDescriptionSet &m,
                         const std::vector<int> &active_ids,
                         int fh) : m_ids(active_ids),
                                   m_buf(100),
                                   m_fh(fh) {
  m_buf[m_buf.size() - 1] = '\0';
  write_field(m_fh, BEGIN_TABLE);
  write_field(m_fh, TR);
  write_field(m_fh, TD);
  write_field(m_fh, "time_ms");
  write_field(m_fh, TDE);

  for (auto i = active_ids.begin(); i != active_ids.end(); ++i) {
    write_field(m_fh, TD);
    for (auto j = m.begin(); j != m.end(); ++j) {
      if (j->id() == *i) {
        write_field(m_fh, j->display_name.c_str());
        break;
      }
    }
    write_field(m_fh, TDE);
  }
  write_field(m_fh, TRE);
}

TableWriter::~TableWriter() {
  write_field(m_fh, END_TABLE);
}

void
TableWriter::Write(const DataSet &d) {
  if (d.empty())
    return;
  write_field(m_fh, TR);

  auto data = d.begin();
  write_field(m_fh, TD);
  snprintf(m_buf.data(), m_buf.size() - 1, "%u", data->time_val);
  write_field(m_fh, m_buf.data());
  write_field(m_fh, TDE);

  for (auto i = m_ids.begin(); i != m_ids.end(); ++i) {
    write_field(m_fh, TD);
    if (data != d.end()) {
      if (data->id == *i) {
        snprintf(m_buf.data(), m_buf.size() - 1, "%f", data->data);
        write_field(m_fh, m_buf.data());
        ++data;
      }
    }
    write_field(m_fh, TDE);
  }
  write(m_fh, TRE, strlen(TRE));
}
