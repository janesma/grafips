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

#include "controls/gfcontrol_stub.h"

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/io/coded_stream.h>

#include <string>

#include "./gfcontrol.pb.h"

using Grafips::ControlStub;
using GrafipsControlProto::ControlInvocation;
using google::protobuf::io::ArrayOutputStream;
using google::protobuf::io::CodedOutputStream;
using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::CodedInputStream;

ControlStub::ControlStub(const std::string &address, int port)
    : m_socket(address, port) {
}
ControlStub::~ControlStub() {
}

bool
ControlStub::Set(const std::string &key, const std::string &value) {
  GrafipsControlProto::ControlInvocation request;

  request.set_method(ControlInvocation::kSet);
  ControlInvocation::Set *args = request.mutable_setargs();
  args->set_key(key);
  args->set_value(value);
  WriteMessage(request);

  GrafipsControlProto::ControlInvocation response;
  ReadResponse(&response);
  assert(response.method() == ControlInvocation::kSetResponse);
  const ControlInvocation::SetResponse &rargs = response.setresponseargs();
  return rargs.status();
}
bool
ControlStub::Get(const std::string &key, std::string *value) {
  GrafipsControlProto::ControlInvocation request;

  request.set_method(ControlInvocation::kGet);
  ControlInvocation::Get *args = request.mutable_getargs();
  args->set_key(key);
  WriteMessage(request);

  GrafipsControlProto::ControlInvocation response;
  ReadResponse(&response);
  assert(response.method() == ControlInvocation::kGetResponse);
  const ControlInvocation::GetResponse &rargs = response.getresponseargs();
  *value = rargs.value();
  return rargs.status();
}

void
ControlStub::AddControl(const std::string &key, ControlInterface* target) {
  // TODO(majanes)
}

void
ControlStub::WriteMessage(const ControlInvocation &m) const {
    const uint32_t write_size = m.ByteSize();
    m_protect.Lock();
    m_socket.Write(write_size);

    m_buf.resize(write_size);
    ArrayOutputStream array_out(m_buf.data(), write_size);
    CodedOutputStream coded_out(&array_out);
    m.SerializeToCodedStream(&coded_out);
    m_socket.Write(m_buf.data(), write_size);
    m_protect.Unlock();
}

void
ControlStub::ReadResponse(GrafipsControlProto::ControlInvocation *m) const {
    m_protect.Lock();
    uint32_t response_size;
    bool status = m_socket.Read(&response_size);
    assert(status);
    assert(response_size);
    m_buf.resize(response_size);

    status = m_socket.ReadVec(&m_buf);
    assert(status);

    const size_t buf_size = m_buf.size();
    ArrayInputStream array_in(m_buf.data(), buf_size);
    CodedInputStream coded_in(&array_in);
    CodedInputStream::Limit msg_limit = coded_in.PushLimit(buf_size);
    m->ParseFromCodedStream(&coded_in);
    coded_in.PopLimit(msg_limit);

    m_protect.Unlock();
}
