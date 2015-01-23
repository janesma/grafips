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

#include "publishers/gfpublisher_remote.h"

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/io/coded_stream.h>

#include <QStringList>

#include "remote/gfipublisher.h"
#include "./gfpublisher.pb.h"
#include "remote/gfmetric.h"
#include "subscriber/gfsubscriber_remote.h"

// #include <iostream>

using Grafips::PublisherStub;

PublisherStub::PublisherStub()
    : m_socket(NULL), m_subscriber(NULL) {
}

void
PublisherStub::Connect() {
    int port = 53135;
    QString address = m_address;

    const int colon_pos = m_address.indexOf(":");
    if (colon_pos != -1) {
        QStringList l = m_address.split(":");
        address = l[0];
        port = l[1].toInt();
    }

    m_socket = new Socket(address.toStdString(), port);
}

PublisherStub::~PublisherStub() {
    // close the socket, which will cause the remote publisher skeleton to
    // close, cleaning up any subscriber stub.
    delete m_socket;
    m_socket = NULL;
    if (m_subscriber) {
        // once the publisher skeleton's subscriber stub is closed, this
        // subscriber skeleton's thread will terminate.
        m_subscriber->Join();
        delete m_subscriber;
        m_subscriber = NULL;
    }
}

void
PublisherStub::WriteMessage(const GrafipsProto::PublisherInvocation &m) const {
    const uint32_t write_size = m.ByteSize();

    m_protect.Lock();
    // std::cout << "write len: " << write_size << std::endl;
    m_socket->Write(write_size);

    m_buf.resize(write_size);
    google::protobuf::io::ArrayOutputStream array_out(m_buf.data(), write_size);
    google::protobuf::io::CodedOutputStream coded_out(&array_out);
    m.SerializeToCodedStream(&coded_out);

    // for (int i = 0; i < write_size; ++i)
        // std::cout << " " << (int) m_buf[i] << " ";
    // std::cout << std::endl;
    m_socket->Write(m_buf.data(), write_size);
    m_protect.Unlock();
}

void
PublisherStub::Enable(int id) {
    GrafipsProto::PublisherInvocation m;
    m.set_method(GrafipsProto::PublisherInvocation::kEnable);
    GrafipsProto::PublisherInvocation::Enable *args = m.mutable_enableargs();
    args->set_id(id);
    WriteMessage(m);
}

void
PublisherStub::Disable(int id) {
    GrafipsProto::PublisherInvocation m;
    m.set_method(GrafipsProto::PublisherInvocation::kDisable);
    GrafipsProto::PublisherInvocation::Disable *args = m.mutable_disableargs();
    args->set_id(id);
    WriteMessage(m);
}

void
PublisherStub::Subscribe(SubscriberInterface *subs) {
    m_subscriber = new SubscriberSkeleton(0, subs);
    const int port = m_subscriber->GetPort();
    m_subscriber->Start();

    GrafipsProto::PublisherInvocation m;
    m.set_method(GrafipsProto::PublisherInvocation::kSubscribe);
    GrafipsProto::PublisherInvocation_Subscribe *args =
        m.mutable_subscribeargs();
    // TODO(majanes): detect local ip and use a real address
    args->set_address("localhost");
    args->set_port(port);

    WriteMessage(m);
}

void
PublisherStub::Flush() const {
    GrafipsProto::PublisherInvocation m;
    m.set_method(GrafipsProto::PublisherInvocation::kFlush);
    WriteMessage(m);

    uint32_t response;
    m_socket->Read(&response);
    assert(response == 0);
}

