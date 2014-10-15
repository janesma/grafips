#include "gtest/gtest.h" 
#include "cpu_provider.h"
#include "publisher.h"

class NullPublisher : public Publisher
{
  public:
    void registerProvider(Provider *p) {}
    void onMetric(const DataSet &d) {}
    void enable(int id) {}
    void getDescriptions(std::vector<MetricDescription> *descriptions) {}
};

class TestPublisher : public Publisher
{
  public:
    void registerProvider(Provider *p) { m_p = p; }
    void onMetric(const DataSet &d) {m_d.insert(m_d.end(), d.begin(), d.end()); }
    void enable(int id) { m_p->enable(id); }
    void getDescriptions(std::vector<MetricDescription> *descriptions) 
        { m_p->getDescriptions(descriptions); }
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
            CpuProvider p(&pub);
            EXPECT_GT(p.m_systemStats.user, 0);
            EXPECT_GT(p.m_systemStats.system, 0);
            EXPECT_GT(p.m_systemStats.idle, 0);

            EXPECT_GT(p.m_coreStats.size(), 0);
            EXPECT_GT(p.m_coreStats[0].user, 0);
            EXPECT_GT(p.m_coreStats[0].system, 0);
            EXPECT_GT(p.m_coreStats[0].idle, 0);
        }

    void test_publish()
        {
            TestPublisher pub;
            CpuProvider p(&pub);
            MetricDescriptionSet metrics;
            pub.getDescriptions(&metrics);
            for (MetricDescriptionSet::iterator i = metrics.begin(); i != metrics.end(); ++i)
                pub.enable(i->id());
            p.poll();
            EXPECT_EQ(pub.m_d.size(), metrics.size());
            for (int i = 0; i < pub.m_d.size(); ++i)
            {
                EXPECT_LT(pub.m_d[i].data, 100);
                EXPECT_GT(pub.m_d[i].data, 0);
                for (int j = i + 1; j < pub.m_d.size(); ++j)
                {
                    EXPECT_NE(pub.m_d[i].id, pub.m_d[j].id);
                    EXPECT_EQ(pub.m_d[i].timeVal, pub.m_d[j].timeVal);
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
