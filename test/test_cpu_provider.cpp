// Copyright (C) Intel Corp.  2014.  All Rights Reserved.

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice (including the
// next paragraph) shall be included in all copies or substantial
// portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//  **********************************************************************/
//  * Authors:
//  *   Mark Janes <mark.a.janes@intel.com>
//  **********************************************************************/

#include <gtest/gtest.h>
#include <QString>

#include <vector>

#include "sources/gfcpu_source.h"
#include "remote/gfpublisher.h"
#include "remote/gfimetric_sink.h"
#include "subscriber/gfsubscriber_remote.h"
#include "remote/gfsubscriber_stub.h"
#include "publishers/gfpublisher_remote.h"
#include "remote/gfpublisher_skel.h"

namespace Grafips {

class NullPublisher : public PublisherInterface, public MetricSinkInterface {
 public:
  void RegisterSource(MetricSourceInterface *p) {}
  void OnMetric(const DataSet &d) {}
  void Activate(int id) {}
  void Deactivate(int id) {}
  void GetDescriptions(std::vector<MetricDescription> *descriptions) const {}
  void Subscribe(SubscriberInterface *s) {}
  void OnDescriptions(const std::vector<MetricDescription> &descriptions) {}
};

class TestCpuPublisher : public PublisherInterface, public MetricSinkInterface {
 public:
  TestCpuPublisher() : m_p(NULL) {}
  void RegisterSource(MetricSourceInterface *p) {
    m_p = p;
    m_p->Subscribe(this);
  }
  void OnMetric(const DataSet &d) {m_d.insert(m_d.end(), d.begin(), d.end()); }
  void Activate(int id) { m_p->Activate(id); }
  virtual void Deactivate(int id) { m_p->Deactivate(id); }
  void Subscribe(SubscriberInterface *s) {}
  void OnDescriptions(const std::vector<MetricDescription> &descriptions) {
    for (MetricDescriptionSet::const_iterator i = descriptions.begin();
         i != descriptions.end(); ++i)
      Activate(i->id());
  }
  DataSet m_d;
  MetricSourceInterface *m_p;
};



class CpuSourceFixture : public testing::Test {
 public:
  CpuSourceFixture() {}

 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}

  void test_parse() {
    NullPublisher pub;
    CpuSource p;
    p.Subscribe(&pub);
    EXPECT_GT(p.m_systemStats.user, 0);
    EXPECT_GT(p.m_systemStats.system, 0);
    EXPECT_GT(p.m_systemStats.idle, 0);

    EXPECT_GT(p.m_core_stats.size(), 0);
    EXPECT_GT(p.m_core_stats[0].user, 0);
    EXPECT_GT(p.m_core_stats[0].system, 0);
    EXPECT_GT(p.m_core_stats[0].idle, 0);
  }

  void test_publish() {
    TestCpuPublisher pub;
    CpuSource p;
    pub.RegisterSource(&p);

    usleep(100000);
    p.Poll();
    for (unsigned int i = 0; i < pub.m_d.size(); ++i) {
      EXPECT_LT(pub.m_d[i].data, 100);
      EXPECT_GE(pub.m_d[i].data, 0);
      for (unsigned int j = i + 1; j < pub.m_d.size(); ++j) {
        EXPECT_NE(pub.m_d[i].id, pub.m_d[j].id);
        EXPECT_EQ(pub.m_d[i].time_val, pub.m_d[j].time_val);
      }
    }
  }
};

TEST_F(CpuSourceFixture, test_parse ) {
  test_parse();
}

TEST_F(CpuSourceFixture, test_publish ) {
  test_publish();
}


class SubscriberMock: public SubscriberInterface {
 public:
  void NotifyDescriptions() {}
  SubscriberMock() : m_cleared(false), m_clear_arg(-1) {}
  void Clear(int id) { m_cleared = true; m_clear_arg = id; }
  void OnMetric(const DataSet &d) { m_d = d; }
  void OnDescriptions(const std::vector<MetricDescription> &descriptions) {
    m_desc = descriptions;
  }
  bool m_cleared;
  int m_clear_arg;
  DataSet m_d;
  std::vector<MetricDescription> m_desc;
};

class RemoteInvokeTest : public testing::Test {
 public:
  RemoteInvokeTest() : m_stub(NULL),
                       m_skel(NULL) {}
 protected:
  virtual void SetUp() {
    m_skel = new SubscriberSkeleton(0, &m_mock);
    m_skel->Start();
    m_stub = new SubscriberStub("localhost", m_skel->GetPort());
  }
  virtual void TearDown() {
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


TEST_F(RemoteInvokeTest, test_call_clear ) {
  ASSERT_FALSE(m_mock.m_cleared);
  m_stub->Clear(5);
  m_stub->Flush();
  EXPECT_TRUE(m_mock.m_cleared);
  EXPECT_EQ(m_mock.m_clear_arg, 5);
}

TEST_F(RemoteInvokeTest, test_call_onmetric) {
  ASSERT_EQ(m_mock.m_d.size(), 0);
  DataSet d;
  d.push_back(DataPoint(1, 1, 1));
  d.push_back(DataPoint(2, 2, 2));
  m_stub->OnMetric(d);
  m_stub->Flush();
  EXPECT_EQ(m_mock.m_d.size(), 2);
  EXPECT_EQ(m_mock.m_d[0].id, 1);
  EXPECT_EQ(m_mock.m_d[1].time_val, 2);
}

TEST_F(RemoteInvokeTest, test_call_ondesc) {
  ASSERT_EQ(m_mock.m_desc.size(), 0);
  std::vector<MetricDescription>  d;
  d.push_back(MetricDescription("one", "/one", "done", GR_METRIC_PERCENT));
  d.push_back(MetricDescription("two", "/two", "dtwo", GR_METRIC_RATE));
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

TEST(RemotePubSub, connect_test) {
  PublisherImpl pub;
  PublisherSkeleton pub_skel(0, &pub);
  pub_skel.Start();
  {
    PublisherStub pub_stub;
    QString address = QString("localhost:%1").arg(pub_skel.GetPort());
    pub_stub.setAddress(address);

    pub_stub.Flush();
    SubscriberMock sub;
    pub_stub.Subscribe(&sub);

    pub_stub.Flush();

    DataSet d;
    d.push_back(DataPoint(1, 1, 1));
    d.push_back(DataPoint(2, 2, 2));
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

TEST(RDTSC, check_time) {
  const unsigned int ms = get_ms_time();
  usleep(100000);
  const unsigned int ms2 = get_ms_time();
  
  EXPECT_LT(abs(ms2 - ms) - 100, 1);
}
}  // namespace Grafips
