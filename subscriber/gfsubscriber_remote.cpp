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

#include "subscriber/gfsubscriber_remote.h"

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/io/coded_stream.h>

#include <vector>

#include "./gfsubscriber.pb.h"
#include "error/gferror.h"
#include "os/gfsocket.h"
#include "remote/gfisubscriber.h"

using Grafips::SubscriberSkeleton;
using GrafipsProto::SubscriberInvocation;

SubscriberSkeleton::SubscriberSkeleton(int port, SubscriberInterface *target)
    : Thread("SubscriberSkeleton"),
      m_server(new ServerSocket(port)),
      m_socket(NULL),
      m_target(target),
      m_running(false) {
}

void
SubscriberSkeleton::Run() {
  m_running = true;
  m_socket = m_server->Accept();
  delete m_server;
  m_server = NULL;
  std::vector<unsigned char> m_buf;
  while (m_running) {
    uint32_t msg_len;
    if (!m_socket->Read(&msg_len))
      return;
    m_buf.resize(msg_len);
    if (!m_socket->ReadVec(&m_buf)) {
      assert(false);
      return;
    }
    const size_t buf_size = m_buf.size();
    google::protobuf::io::ArrayInputStream array_in(m_buf.data(), buf_size);
    google::protobuf::io::CodedInputStream coded_in(&array_in);

    GrafipsProto::SubscriberInvocation m;
    google::protobuf::io::CodedInputStream::Limit msg_limit =
        coded_in.PushLimit(buf_size);
    m.ParseFromCodedStream(&coded_in);
    coded_in.PopLimit(msg_limit);

    typedef GrafipsProto::SubscriberInvocation_ClearM ClearM;
    typedef GrafipsProto::SubscriberInvocation_OnMetric OnMetric;
    typedef SubscriberInvocation::OnDescriptions OnDescriptions;
    switch (m.method()) {
      case GrafipsProto::SubscriberInvocation::kFlush: {
        if (!m_socket->Write((uint32_t)0))
          // wrote to closed socket
          m_running = false;
        break;
      }
      case GrafipsProto::SubscriberInvocation::kClear: {
        const ClearM& args= m.clearargs();

        m_target->Clear(args.id());
        // m_socket->Write((uint32_t)0);
        break;
      }
      case GrafipsProto::SubscriberInvocation::kOnMetric: {
        const OnMetric& args = m.onmetricargs();
        const int count = args.data_size();
        DataSet d_set;
        for (int i = 0; i < count; ++i) {
          const ::GrafipsProto::DataPoint &in_d = args.data(i);
          d_set.push_back(DataPoint(in_d.time_val(), in_d.id(),
                                    in_d.data()));
        }
        m_target->OnMetric(d_set);
        // m_socket->Write((uint32_t)0);
        break;
      }
      case GrafipsProto::SubscriberInvocation::kOnDescriptions: {
        const OnDescriptions& args = m.ondescriptionsargs();
        const int count = args.descriptions_size();
        std::vector<MetricDescription> d_set;
        for (int i = 0; i < count; ++i) {
          const ::GrafipsProto::MetricDescription
              &in_d(args.descriptions(i));

          d_set.push_back(MetricDescription(in_d.path(),
                                            in_d.help_text(),
                                            in_d.display_name(),
                                            (MetricType)in_d.type(),
                                            in_d.enabled()));
        }
        m_target->OnDescriptions(d_set);
        break;
      }
      default:
        assert(false);
        return;
    }
  }
}

void
SubscriberSkeleton::Stop() {
  m_running = false;
}


SubscriberSkeleton::~SubscriberSkeleton() {
  if (m_socket)
    delete m_socket;
  if (m_server)
    delete m_server;
}

int
SubscriberSkeleton::GetPort() const {
  assert(m_server);
  return m_server->GetPort();
}
