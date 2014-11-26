#include "gfthread.h"

#include <assert.h>

// This wrapper uses pthread instead of std::thread, because std::thread throws
// an exception when running in a fips-preloaded application.

using namespace Grafips;

Thread::Thread(const std::string &name) : m_name(name)
{}

void *start_thread(void*ctx);
void *start_thread(void*ctx)
{
    ((Thread*)ctx)->Run();
    return NULL;
}

void
Thread::Start()
{
    const int result = pthread_create(&m_thread, NULL, &start_thread, this);
    assert(result == 0);
}

void
Thread::Join()
{
    pthread_join(m_thread, NULL);
}


