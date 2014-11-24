#include "gtest/gtest.h" 

#include <QString>

#include "gfcpu_provider.h"
#include "gfpublisher.h"
#include "gfsubscriber_remote.h"
#include "gfpublisher_remote.h"
#include "gfpublisher_skel.h"

namespace Grafips
{

    class NullPublisher : public PublisherInterface
    {
      public:
        void RegisterProvider(Provider *p) {}
        void OnMetric(const DataSet &d) {}
        void Enable(int id) {}
        void Disable(int id) {}
        void GetDescriptions(std::vector<MetricDescription> *descriptions) const {}
        void Subscribe(SubscriberInterface *) {}
    };

    class TestPublisher : public PublisherInterface
    {
      public:
        void RegisterProvider(Provider *p) { m_p = p; }
        void OnMetric(const DataSet &d) {m_d.insert(m_d.end(), d.begin(), d.end()); }
        void Enable(int id) { m_p->Enable(id); }
        virtual void Disable(int id) { m_p->Disable(id); }
        void GetDescriptions(std::vector<MetricDescription> *descriptions) const
            { m_p->GetDescriptions(descriptions); }
        void Subscribe(SubscriberInterface *) {}
        DataSet m_d;
        Provider *m_p;
    };



    class CpuProviderFixture : public testing::Test
    {
      public:
        CpuProviderFixture() 
            {
            }
      protected:
        virtual void SetUp() {}
        virtual void TearDown() {}

        void test_parse()
            {
                NullPublisher pub;
                CpuProvider p;
                p.setPublisher(&pub);
                EXPECT_GT(p.m_systemStats.user, 0);
                EXPECT_GT(p.m_systemStats.system, 0);
                EXPECT_GT(p.m_systemStats.idle, 0);

                EXPECT_GT(p.m_core_stats.size(), 0);
                EXPECT_GT(p.m_core_stats[0].user, 0);
                EXPECT_GT(p.m_core_stats[0].system, 0);
                EXPECT_GT(p.m_core_stats[0].idle, 0);
            }

        void test_publish()
            {
                TestPublisher pub;
                CpuProvider p;
                p.setPublisher(&pub);

                MetricDescriptionSet metrics;
                pub.GetDescriptions(&metrics);
                for (MetricDescriptionSet::iterator i = metrics.begin(); i != metrics.end(); ++i)
                    pub.Enable(i->id());
                usleep(100000);
                p.Poll();
                EXPECT_EQ(pub.m_d.size(), metrics.size());
                for (int i = 0; i < pub.m_d.size(); ++i)
                {
                    EXPECT_LT(pub.m_d[i].data, 100);
                    EXPECT_GE(pub.m_d[i].data, 0);
                    for (int j = i + 1; j < pub.m_d.size(); ++j)
                    {
                        EXPECT_NE(pub.m_d[i].id, pub.m_d[j].id);
                        EXPECT_EQ(pub.m_d[i].time_val, pub.m_d[j].time_val);
                    }
                }
            }
    };

    TEST_F(CpuProviderFixture, test_parse )
    {
        test_parse();
    }

    TEST_F(CpuProviderFixture, test_publish )
    {
        test_publish();
    }


    class SubscriberMock: public SubscriberInterface
    {
      public:
        void NotifyDescriptions() {}
        SubscriberMock() : m_cleared(false), m_clear_arg(-1) {}
        void Clear(int id) { m_cleared = true; m_clear_arg = id; }
        void OnMetric(const DataSet &d) { m_d = d; }
        void OnDescriptions(const std::vector<MetricDescription> &descriptions) { m_desc = descriptions; }
        bool m_cleared;
        int m_clear_arg;
        DataSet m_d;
        std::vector<MetricDescription> m_desc;
    };

    class RemoteInvokeTest : public testing::Test
    {
      public:
        RemoteInvokeTest() : m_stub(NULL),
                             m_skel(NULL) {}
      protected:
        virtual void SetUp()
            {
                m_skel = new SubscriberSkeleton(0, &m_mock);
                m_skel->Start();
                m_stub = new SubscriberStub("localhost", m_skel->GetPort());
            }
        virtual void TearDown()
            {
                m_skel->Stop();

                // send message to trigger server stop
                delete m_stub;

                m_skel->Join();
                delete m_skel;
            }

        SubscriberStub *m_stub;
        SubscriberSkeleton *m_skel;
        SubscriberMock m_mock;
    };


    TEST_F(RemoteInvokeTest, test_call_clear )
    {
        ASSERT_FALSE(m_mock.m_cleared);
        m_stub->Clear(5);
        m_stub->Flush();
        EXPECT_TRUE(m_mock.m_cleared);
        EXPECT_EQ(m_mock.m_clear_arg, 5);
    }

    TEST_F(RemoteInvokeTest, test_call_onmetric)
    {
        ASSERT_EQ(m_mock.m_d.size(), 0);
        DataSet d;
        d.push_back(DataPoint(1,1,1));
        d.push_back(DataPoint(2,2,2));
        m_stub->OnMetric(d);
        m_stub->Flush();
        EXPECT_EQ(m_mock.m_d.size(), 2);
        EXPECT_EQ(m_mock.m_d[0].id, 1);
        EXPECT_EQ(m_mock.m_d[1].time_val, 2);
    }

    TEST_F(RemoteInvokeTest, test_call_ondesc)
    {
        ASSERT_EQ(m_mock.m_desc.size(), 0);
        std::vector<MetricDescription>  d;
        d.push_back(MetricDescription("one","/one","done", GR_METRIC_PERCENT));
        d.push_back(MetricDescription("two","/two","dtwo", GR_METRIC_RATE));
        m_stub->OnDescriptions(d);
        m_stub->Flush();
        EXPECT_EQ(m_mock.m_desc.size(), 2);
        EXPECT_STREQ(m_mock.m_desc[0].path.c_str(), "one");
        EXPECT_STREQ(m_mock.m_desc[0].help_text.c_str(), "/one");
        EXPECT_STREQ(m_mock.m_desc[0].display_name.c_str(), "done");
        EXPECT_EQ(m_mock.m_desc[0].type, GR_METRIC_PERCENT);
        EXPECT_STREQ(m_mock.m_desc[1].path.c_str(), "two");
        EXPECT_STREQ(m_mock.m_desc[1].help_text.c_str(), "/two");
        EXPECT_STREQ(m_mock.m_desc[1].display_name.c_str(), "dtwo");
        EXPECT_EQ(m_mock.m_desc[1].type, GR_METRIC_RATE);
    }

    TEST(RemotePubSub, connect_test)
    {
        PublisherImpl pub;
        PublisherSkeleton pub_skel(0, &pub);
        pub_skel.Start();

        {
            PublisherStub pub_stub;
            QString address = QString("localhost:%1").arg(pub_skel.GetPort());
            pub_stub.setAddress(address);

            pub_stub.Flush();
            
            std::vector<MetricDescription> desc;
            pub_stub.GetDescriptions(&desc);
            
            SubscriberMock sub;
            pub_stub.Subscribe(&sub);

            pub_stub.Flush();

            DataSet d;
            d.push_back(DataPoint(1,1,1));
            d.push_back(DataPoint(2,2,2));
            pub.OnMetric(d);

            // need to ensure that the ephemeral subscriber stub has fully
            // delivered it's publication, and that it was delivered.  This
            // stub was created within the PublisherSkeleton when subscribe was
            // called.
            pub_skel.Flush();

            EXPECT_EQ(sub.m_d.size(), 2);
            EXPECT_EQ(sub.m_d[0].id, 1);
            EXPECT_EQ(sub.m_d[1].time_val, 2);

            // pub_stub destructor should make publisher skeleton finish
        }

        pub_skel.Join();
    }
}
