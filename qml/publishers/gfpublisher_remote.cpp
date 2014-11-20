#include "gfpublisher_remote.h"

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/io/coded_stream.h>

#include "gfipublisher.h"
#include "gfpublisher.pb.h"
#include "gfmetric.h"
#include "gfsubscriber_remote.h"

using namespace Grafips;

PublisherStub::PublisherStub(const std::string &address, int port)
    : m_socket(new Socket(address, port)), m_subscriber(NULL)
{
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
    m_socket->Write(write_size);
    
    m_buf.resize(write_size);
    google::protobuf::io::ArrayOutputStream array_out(m_buf.data(), write_size);
    google::protobuf::io::CodedOutputStream coded_out(&array_out);
    m.SerializeToCodedStream(&coded_out);
    m_socket->Write(m_buf.data(), write_size);
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
    const uint32_t write_size = m.ByteSize();
    m_socket->Write(write_size);
    
    m_buf.resize(write_size);
    google::protobuf::io::ArrayOutputStream array_out(m_buf.data(), write_size);
    google::protobuf::io::CodedOutputStream coded_out(&array_out);
    m.SerializeToCodedStream(&coded_out);
    m_socket->Write(m_buf.data(), write_size);

    uint32_t response;
    m_socket->Read(&response);
    assert(response == 0);
}

PublisherSkeleton::PublisherSkeleton(int port, PublisherInterface *target)
    : Thread("PublisherSkeleton"), m_server(new ServerSocket(port)), m_target(target),
      m_socket(NULL), m_subscriber(NULL)
{
}

PublisherSkeleton::~PublisherSkeleton()
{
    if (m_socket)
        delete m_socket;
    if (m_server)
        delete m_server;
}

void
PublisherSkeleton::Run()
{
    m_socket = m_server->Accept();
    delete m_server;
    m_server = NULL;

    std::vector<unsigned char> m_buf;
    bool running = true;
    while (running)
    {
        uint32_t msg_len;
        if (! m_socket->Read(&msg_len))
            break;

        m_buf.resize(msg_len);
        if (! m_socket->ReadVec(&m_buf))
            break;


        const size_t buf_size = m_buf.size();
        google::protobuf::io::ArrayInputStream array_in(m_buf.data(), buf_size);
        google::protobuf::io::CodedInputStream coded_in(&array_in);
        
        GrafipsProto::PublisherInvocation m;
        google::protobuf::io::CodedInputStream::Limit msg_limit = coded_in.PushLimit(buf_size);
        m.ParseFromCodedStream(&coded_in);
        coded_in.PopLimit(msg_limit);

        switch (m.method())
        {
            case GrafipsProto::PublisherInvocation::kFlush:
                {
                    m_socket->Write((uint32_t)0);
                    break;
                }
            case GrafipsProto::PublisherInvocation::kEnable:
                {
                    const GrafipsProto::PublisherInvocation_Enable& args= m.enableargs();
                    m_target->Enable(args.id());
                    break;
                }
            case GrafipsProto::PublisherInvocation::kDisable:
                {
                    const GrafipsProto::PublisherInvocation_Disable& args= m.disableargs();
                    m_target->Disable(args.id());
                    break;
                }
            case GrafipsProto::PublisherInvocation::kGetDescriptions:
                {
                    std::vector<MetricDescription> d;
                    m_target->GetDescriptions(&d);

                    GrafipsProto::PublisherInvocation response;
                    m.set_method(GrafipsProto::PublisherInvocation::kGetDescriptions);
                    GrafipsProto::PublisherInvocation::GetDescriptions * args = response.mutable_getdescriptionsargs();
                    for (std::vector<MetricDescription>::const_iterator i = d.begin();
                         i != d.end(); ++i)
                    {
                        ::GrafipsProto::MetricDescription *pdesc =  args->add_descriptions();
                        pdesc->set_path(i->path);
                        pdesc->set_help_text(i->help_text);
                        pdesc->set_display_name(i->display_name);
                        pdesc->set_type((::GrafipsProto::MetricType)i->type);
                    }
                    WriteMessage(response);
                    break;
                }
            case GrafipsProto::PublisherInvocation::kSubscribe:
                {
                    assert (m_subscriber == NULL);
                    const GrafipsProto::PublisherInvocation_Subscribe& args = m.subscribeargs();
                    m_subscriber = new SubscriberStub(args.address(), args.port());
                    m_target->Subscribe(m_subscriber);
                    break;
                }
            default:
                {
                    assert(false);
                    running = false;
                    break;
                }
        }
    }

    // clean up subscriber stub
    if (m_subscriber)
    {
        delete m_subscriber;
        m_subscriber = NULL;
    }
}

void
PublisherSkeleton::WriteMessage(const GrafipsProto::PublisherInvocation &m)
{
    const uint32_t write_size = m.ByteSize();
    m_socket->Write(write_size);
    
    m_buf.resize(write_size);
    google::protobuf::io::ArrayOutputStream array_out(m_buf.data(), write_size);
    google::protobuf::io::CodedOutputStream coded_out(&array_out);
    m.SerializeToCodedStream(&coded_out);
    m_socket->Write(m_buf.data(), write_size);
}

void
PublisherSkeleton::Flush() const
{
    if (m_subscriber)
        m_subscriber->Flush();
}

int
PublisherSkeleton::GetPort() const
{
    return m_server->GetPort();
}
