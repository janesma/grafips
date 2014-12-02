#pragma once


#include <QObject>

#include "os/gfthread.h"

namespace GrafipsProto
{
    class SubscriberInvocation;
    class SubscriberInterface;
}

namespace Grafips
{
    class Socket;
    class ServerSocket;
    class SubscriberInterface    ;
    
    class SubscriberSkeleton : public QObject, public Thread
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
