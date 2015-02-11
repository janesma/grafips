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

void
ControlStub::Set(const std::string &key, const std::string &value) {
  GrafipsControlProto::ControlInvocation request;

  request.set_method(ControlInvocation::kSet);
  ControlInvocation::Set *args = request.mutable_setargs();
  args->set_key(key);
  args->set_value(value);
  WriteMessage(request);

  // asynchronous, no response
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

