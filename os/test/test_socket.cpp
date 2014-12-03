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

#include <stdlib.h>

#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "os/gfthread.h"
#include "os/gfsocket.h"

using Grafips::ServerSocket;
using Grafips::Thread;
using Grafips::Socket;

class EchoServerThread : public Thread {
 public:
  explicit EchoServerThread(int port)
      : Thread("echoserver"), m_running(true),
        m_sock(NULL), m_server_sock(port) {}
  void Run() {
    std::vector<unsigned char> buf;
    m_sock = m_server_sock.Accept();
    while (m_running) {
      uint32_t msg_len;
      bool success = m_sock->Read(&msg_len);
      if (!success)
        break;

      buf.resize(msg_len);
      success = m_sock->Read(buf.data(), msg_len);
      ASSERT_TRUE(success);

      success = m_sock->Write(msg_len);
      ASSERT_TRUE(success);
      success = m_sock->Write(buf.data(), buf.size());
      ASSERT_TRUE(success);
    }
    delete m_sock;
  }
  void Stop() {
    m_running = false;
  }
  int GetPort() const { return m_server_sock.GetPort(); }

 private:
  bool m_running;
  Socket *m_sock;
  ServerSocket m_server_sock;
};

class SocketFixture : public testing::Test {
 public:
  SocketFixture() : m_server_thread(0) {}
  void SetUp() {
    m_server_thread.Start();
  }
  void TearDown() {
    m_server_thread.Stop();
    m_server_thread.Join();
  }
 protected:
  EchoServerThread m_server_thread;
};


TEST_F(SocketFixture, Instantiate) {
  Socket client("localhost", m_server_thread.GetPort());
  int eof = 0;
  ASSERT_TRUE(client.Write(&eof, sizeof(eof)));
  ASSERT_TRUE(client.Read(&eof, sizeof(eof)));
  ASSERT_EQ(eof, 0);
}

TEST_F(SocketFixture, Echo) {
  Socket client("localhost", m_server_thread.GetPort());
  std::string hello("this is a test");
  ASSERT_TRUE(client.Write((uint32_t) hello.size()));
  ASSERT_TRUE(client.Write(hello.c_str(), hello.size()));

  uint32_t bytes;
  std::vector<char> buf(hello.size());
  ASSERT_TRUE(client.Read(&bytes));
  ASSERT_EQ(bytes, hello.size());
  ASSERT_TRUE(client.Read(buf.data(), hello.size()));
  ASSERT_EQ(0, strncmp(buf.data(), hello.c_str(), hello.size()));
}

TEST_F(SocketFixture, RandomData) {
  Socket client("localhost", m_server_thread.GetPort());
  srand(time(NULL));
  std::string hello("this is a test");

  const float F_RAND_MAX = RAND_MAX;
  unsigned int seed = time(NULL);
  const float f_rand = rand_r(&seed);
  int reps = (f_rand  / F_RAND_MAX) * 100.0;
  std::vector<int> data, echo_data;
  while (--reps > 0) {
    const float f_rand = rand_r(&seed);
    const int buf_size = (f_rand / F_RAND_MAX) * 100.0;
    data.resize(buf_size);
    for (std::vector<int>::iterator d = data.begin(); d != data.end(); ++d)
      *d = rand_r(&seed);
    client.Write((uint32_t) (buf_size * sizeof(int)));
    client.WriteVec(data);

    uint32_t echo_size;
    client.Read(&echo_size);
    echo_data.resize(echo_size / sizeof(int));
    client.ReadVec(&echo_data);

    ASSERT_EQ(echo_data, data);
  }
}


// test cases:
// - normal start/stop
//   - server stops
//   - client stops
// - client drops unexpectedly
// - server drops unexpectedly
// - client never connects
// - client connects to nonexistent server
