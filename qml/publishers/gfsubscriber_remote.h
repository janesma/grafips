#pragma once

#include <string>
#include <vector>

#include "gfisubscriber.h"
#include "gfsocket.h"
#include "gfthread.h"

namespace GrafipsProto
{
    class SubscriberInvocation;
}

namespace Grafips
{
    class SubscriberStub : public SubscriberInterface
    {
      public:
        SubscriberStub(const std::string &address, int port);
        ~SubscriberStub();
        void Clear(int id);
        void OnMetric(const DataSet &d);
        void OnDescriptions(const std::vector<MetricDescription> &descriptions);
        void Flush() const;
      private:
        void WriteMessage(const GrafipsProto::SubscriberInvocation&m) const;
        mutable Socket m_socket;
        mutable std::vector<unsigned char> m_buf;
    };


    class SubscriberSkeleton : public Thread
    {
      public:
        SubscriberSkeleton(int port, SubscriberInterface *target);
        ~SubscriberSkeleton();
        void Run();
        void Stop();
        int GetPort() const;
      private:
        ServerSocket *m_server;
        Socket *m_socket;
        SubscriberInterface *m_target;
        bool m_running;
    };
}
