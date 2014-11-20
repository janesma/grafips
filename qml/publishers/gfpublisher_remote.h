#pragma once
#include <string>
#include <vector>

#include "gfthread.h"
#include "gfmetric.h"
#include "gfsocket.h"


namespace GrafipsProto
{
    class PublisherInvocation;
}

namespace Grafips
{
    class SubscriberInterface;
    class PublisherInterface;
    class SubscriberStub;
    class SubscriberSkeleton;

    class PublisherStub
    {
      public:
        PublisherStub(const std::string &address, int port);
        ~PublisherStub();
        
        void Enable(int id);
        void Disable(int id);
        void GetDescriptions(std::vector<MetricDescription> *descriptions) const;
        void Subscribe(SubscriberInterface *);

        void Flush() const;
      private:
        void WriteMessage(const GrafipsProto::PublisherInvocation &m) const;
        mutable Socket *m_socket;
        mutable std::vector<unsigned char> m_buf;
        SubscriberSkeleton *m_subscriber;
    };
    
    class PublisherSkeleton : public Thread
    {
      public:
        PublisherSkeleton(int port, PublisherInterface *target);
        ~PublisherSkeleton();
        void Run();
        void Flush() const;
        int GetPort() const;
      private:
        void WriteMessage(const GrafipsProto::PublisherInvocation &m);
        ServerSocket *m_server;
        Socket *m_socket;
        PublisherInterface *m_target;

        std::vector<unsigned char> m_buf;

        // on Subscribe(), this member is created to send publications remotely
        SubscriberStub *m_subscriber;

     };
}
