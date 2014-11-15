#include "gfsubscriber_remote.h"

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/io/coded_stream.h>

#include "gfsubscriber.pb.h"

using namespace Grafips;

SubscriberStub::SubscriberStub(const std::string &address,
                               int port)
    : m_socket(address, port)
{
}

SubscriberStub::~SubscriberStub()
{
}

void
SubscriberStub::Clear(int id)
{
    GrafipsProto::SubscriberInvocation m;
    m.set_method(GrafipsProto::SubscriberInvocation::kClear);
    GrafipsProto::SubscriberInvocation::ClearM * args = m.mutable_clearargs();
    args->set_id(id);

    const uint32_t write_size = m.ByteSize();
    m_socket.Write(write_size);
    
    m_buf.resize(write_size);
    google::protobuf::io::ArrayOutputStream array_out(m_buf.data(), write_size);
    google::protobuf::io::CodedOutputStream coded_out(&array_out);
    m.SerializeToCodedStream(&coded_out);
    m_socket.Write(m_buf.data(), write_size);

    // asynchronous, no response
    uint32_t response;
    m_socket.Read(&response);
    assert(response == 0);
}

// TODO
void
SubscriberStub::OnMetric(const DataSet &d) {}
void
SubscriberStub::OnDescriptions(const std::vector<MetricDescription> &descriptions){}

SubscriberSkeleton::SubscriberSkeleton(int port, SubscriberInterface *target)
    : Thread("SubscriberSkeleton"),
      m_server(new ServerSocket(port)),
      m_socket(NULL),
      m_target(target),
      m_running(false)
{
}

void
SubscriberSkeleton::Run()
{
    m_running = true;
    m_socket = m_server->Accept();
    delete m_server;
    m_server = NULL;
    std::vector<unsigned char> m_buf;
    while (m_running)
    {
        uint32_t msg_len;
        if (! m_socket->Read(&msg_len))
            return;
        m_buf.resize(msg_len);
        if (! m_socket->ReadVec(&m_buf))
            return;

        const size_t buf_size = m_buf.size();
        google::protobuf::io::ArrayInputStream array_in(m_buf.data(), buf_size);
        google::protobuf::io::CodedInputStream coded_in(&array_in);
        
        GrafipsProto::SubscriberInvocation m;
        google::protobuf::io::CodedInputStream::Limit msg_limit = coded_in.PushLimit(buf_size);
        m.ParseFromCodedStream(&coded_in);
        coded_in.PopLimit(msg_limit);

        switch (m.method())
        {
            case GrafipsProto::SubscriberInvocation::kClear:
                {
                    const GrafipsProto::SubscriberInvocation_ClearM& args= m.clearargs();
                    m_target->Clear(args.id());
                    m_socket->Write((uint32_t)0);
                    break;
                }
            default:
                return;
        }
    }
}

void
SubscriberSkeleton::Stop()
{
    m_running = false;
}


SubscriberSkeleton::~SubscriberSkeleton()
{
    if (m_socket)
        delete m_socket;
    if (m_server)
        delete m_server;
}
