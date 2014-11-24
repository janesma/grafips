#pragma once
#include <string>
#include <vector>
#include <assert.h>
#include <mutex>

#include <QObject>

#include "gfmetric.h"
#include "gfsocket.h"
#include "gfipublisher.h"


namespace GrafipsProto
{
    class PublisherInvocation;
}

namespace Grafips
{
    class SubscriberInterface;
    class SubscriberStub;
    class SubscriberSkeleton;

    class PublisherStub : public QObject, public PublisherInterface
    {
        Q_OBJECT
        Q_PROPERTY(QString address READ address WRITE setAddress)
      public:
        PublisherStub();
        ~PublisherStub();
        
        Q_INVOKABLE void Enable(int id);
        Q_INVOKABLE void Disable(int id);
        void GetDescriptions(std::vector<MetricDescription> *descriptions) const;
        Q_INVOKABLE void Subscribe(SubscriberInterface *);

        void Flush() const;

        QString address() const { return m_address; }
        void setAddress(const QString &a) { m_address = a; Connect(); }

        // todo: try to remove
        void RegisterProvider(Provider *p) { assert (false); }
        void OnMetric(const DataSet &d) { assert (false); }
      private:
        void WriteMessage(const GrafipsProto::PublisherInvocation &m) const;
        void Connect();
        
        mutable Socket *m_socket;
        mutable std::vector<unsigned char> m_buf;
        mutable std::mutex m_protect;
        SubscriberSkeleton *m_subscriber;
        QString m_address;
    };
}
