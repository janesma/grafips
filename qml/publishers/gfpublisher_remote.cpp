#include "gfpublisher_remote.h"

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/io/coded_stream.h>

#include <QStringList>

#include "gfipublisher.h"
#include "gfpublisher.pb.h"
#include "gfmetric.h"
#include "gfsubscriber_remote.h"

// #include <iostream>

using namespace Grafips;

PublisherStub::PublisherStub()
    : m_socket(NULL), m_subscriber(NULL)
{
}

void
PublisherStub::Connect()
{
    int port = 53135;
    QString address = m_address;

    const int colon_pos = m_address.indexOf(":");
    if (colon_pos != -1)
    {
        QStringList l = m_address.split(":");
        address = l[0];
        port = l[1].toInt();
    }
        
    m_socket = new Socket(address.toStdString(), port);
}

PublisherStub::~PublisherStub()
{
    // close the socket, which will cause the remote publisher skeleton to
    // close, cleaning up any subscriber stub.
    delete m_socket;
    m_socket = NULL;
    if (m_subscriber)
    {
        // once the publisher skeleton's subscriber stub is closed, this
        // subscriber skeleton's thread will terminate.
        m_subscriber->Join();
        delete m_subscriber;
        m_subscriber = NULL;
    }
}

void
PublisherStub::WriteMessage(const GrafipsProto::PublisherInvocation &m) const
{
    const uint32_t write_size = m.ByteSize();

    m_protect.lock();
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
    m_protect.unlock();
}

void
PublisherStub::Enable(int id)
{
    GrafipsProto::PublisherInvocation m;
    m.set_method(GrafipsProto::PublisherInvocation::kEnable);
    GrafipsProto::PublisherInvocation::Enable *args = m.mutable_enableargs();
    args->set_id(id);
    WriteMessage(m);
}

void
PublisherStub::Disable(int id)
{
    GrafipsProto::PublisherInvocation m;
    m.set_method(GrafipsProto::PublisherInvocation::kDisable);
    GrafipsProto::PublisherInvocation::Disable *args = m.mutable_disableargs();
    args->set_id(id);
    WriteMessage(m);
}

void
PublisherStub::GetDescriptions(std::vector<MetricDescription> *descriptions) const
{
    GrafipsProto::PublisherInvocation m;
    m.set_method(GrafipsProto::PublisherInvocation::kGetDescriptions);
    WriteMessage(m);

    uint32_t msg_len;
    if (! m_socket->Read(&msg_len))
    {
        assert (false);
        return;
    }
    m_buf.resize(msg_len);
    if (! m_socket->ReadVec(&m_buf))
    {
        assert (false);
        return;
    }
    google::protobuf::io::ArrayInputStream array_in(m_buf.data(), msg_len);
    google::protobuf::io::CodedInputStream coded_in(&array_in);

    google::protobuf::io::CodedInputStream::Limit msg_limit = coded_in.PushLimit(msg_len);
    m.ParseFromCodedStream(&coded_in);
    coded_in.PopLimit(msg_limit);
    assert(m.method() == GrafipsProto::PublisherInvocation::kGetDescriptions);

    const GrafipsProto::PublisherInvocation_GetDescriptions &args = m.getdescriptionsargs();
    const int count = args.descriptions_size();
    descriptions->clear();
    for (int i = 0; i < count; ++i)
    {
        const GrafipsProto::MetricDescription &in_d = args.descriptions(i);
        descriptions->push_back(MetricDescription(in_d.path(),
                                                  in_d.help_text(),
                                                  in_d.display_name(),
                                                  (Grafips::MetricType) in_d.type()));
    }
}

void
PublisherStub::Subscribe(SubscriberInterface *subs)
{
    m_subscriber = new SubscriberSkeleton(0, subs);
    const int port = m_subscriber->GetPort();
    m_subscriber->Start();

    GrafipsProto::PublisherInvocation m;
    m.set_method(GrafipsProto::PublisherInvocation::kSubscribe);
    GrafipsProto::PublisherInvocation_Subscribe *args = m.mutable_subscribeargs();
    args->set_address("localhost");
    args->set_port(port);

    WriteMessage(m);
}

void
PublisherStub::Flush() const
{
    GrafipsProto::PublisherInvocation m;
    m.set_method(GrafipsProto::PublisherInvocation::kFlush);
    WriteMessage(m);

    uint32_t response;
    m_socket->Read(&response);
    assert(response == 0);
}

