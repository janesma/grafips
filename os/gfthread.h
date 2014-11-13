#pragma once

#include <string>
#include <thread>

namespace Grafips
{
    class Thread
    {
      public:
        Thread(const std::string &name);
        virtual void Run() = 0;
        void Start();
        void Join();
      private:
        const std::string m_name;
        std::thread *m_thread;
    };
}
