#pragma once
#include <string>
#include <vector>

#include "gfthread.h"
#include "gfmetric.h"
#include "gfsocket.h"

class SubscriberInterface;
class PublisherInterface;
class SubscriberStub;

namespace GrafipsProto
{
    class PublisherInvocation;
}

namespace Grafips
{
    class PublisherStub
    {
      public:
        PublisherStub(const std::string &address, int port);
        ~PublisherStub();
        
        void Enable(int id);
        void Disable(int id);
        void GetDescriptions(std::vector<MetricDescription> *descriptions) const;
        void Subscribe(SubscriberInterface *);
      private:
        void WriteMessage(const GrafipsProto::PublisherInvocation &m);
        Socket m_socket;
        std::vector<unsigned char> m_buf;
    };
    
    class PublisherSkeleton : public Thread
    {
        PublisherSkeleton(int port, PublisherInterface *target);
        ~PublisherSkeleton();
        void Run();
        void Stop();
      private:
        ServerSocket *m_server;
        Socket *m_socket;
        PublisherInterface *m_target;
        bool m_running;

        // on Subscribe(), this member is created to send publications remotely
        SubscriberStub *m_subscriber;
     };
}
