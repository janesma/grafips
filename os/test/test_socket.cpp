#include <stdlib.h>

#include "gtest/gtest.h" 
#include "gfthread.h"
#include "gfsocket.h"

using namespace Grafips;

class EchoServerThread : public Thread
{
  public:
    EchoServerThread(int port) : Thread("echoserver"), m_running(true),
                                 m_sock(NULL), m_server_sock(port) {}
    void Run()
        {
            std::vector<unsigned char> buf;
            m_sock = m_server_sock.Accept();
            while (m_running)
            {
                uint32_t msg_len;
                bool success = m_sock->Read(&msg_len);
                if (! success)
                    break;

                buf.resize(msg_len);
                success = m_sock->Read(buf.data(),msg_len);
                ASSERT_TRUE(success);

                success = m_sock->Write(msg_len);
                ASSERT_TRUE(success);
                success = m_sock->Write(buf.data(), buf.size());
                ASSERT_TRUE(success);
            }
            delete m_sock;
        }
    void Stop()
        {
            m_running = false;
        }
    int GetPort() const { return m_server_sock.GetPort(); }
  private:
    bool m_running;
    Socket *m_sock;
    ServerSocket m_server_sock;
};

class SocketFixture : public testing::Test
{
  public:
    SocketFixture() : m_server_thread(0) {}
    void SetUp()
        {
            m_server_thread.Start();
        }
    void TearDown()
        {
            m_server_thread.Stop();
            m_server_thread.Join();
        }
  protected:
    EchoServerThread m_server_thread;
};


TEST_F(SocketFixture, Instantiate)
{
    Socket client("localhost", m_server_thread.GetPort());
    int eof = 0;
    ASSERT_TRUE(client.Write(&eof, sizeof(eof)));
    ASSERT_TRUE(client.Read(&eof, sizeof(eof)));
    ASSERT_EQ(eof, 0);
}

TEST_F(SocketFixture, Echo)
{
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

TEST_F(SocketFixture, RandomData)
{
    Socket client("localhost", m_server_thread.GetPort());
    srand(time(NULL));
    std::string hello("this is a test");

    int reps = (int)(((float) rand() / (float)RAND_MAX ) * (float) 100);
    std::vector<int> data, echo_data;
    while (--reps > 0)
    {
        const int buf_size = (int)(((float) rand() / (float)RAND_MAX ) * (float) 100);
        data.resize(buf_size);
        for (std::vector<int>::iterator d = data.begin(); d != data.end(); ++d)
            *d = rand();
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
