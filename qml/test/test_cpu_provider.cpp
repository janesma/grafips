#include "gtest/gtest.h" 
#include "cpu_provider.h"

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
            CpuProvider p;
            EXPECT_GT(p.m_systemStats.user, 0);
            EXPECT_GT(p.m_systemStats.system, 0);
            EXPECT_GT(p.m_systemStats.idle, 0);

            EXPECT_GT(p.m_coreStats.size(), 0);
            EXPECT_GT(p.m_coreStats[0].user, 0);
            EXPECT_GT(p.m_coreStats[0].system, 0);
            EXPECT_GT(p.m_coreStats[0].idle, 0);
        }
};

TEST_F(CpuProviderFixture, test_parse )
{
    test_parse();
}
