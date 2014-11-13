#include "gfthread.h"

#include <thread>

using namespace Grafips;

Thread::Thread(const std::string &name) : m_name(name)
{}

void
Thread::Start()
{
    m_thread = new std::thread(&Thread::Run, this);
}

void
Thread::Join()
{
    m_thread->join();
    delete m_thread;
    m_thread = NULL;
}


