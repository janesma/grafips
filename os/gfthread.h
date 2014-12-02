#pragma once

#include <pthread.h>
#include <string>

#include "gftraits.h"

namespace Grafips
{
    class Thread : NoCopy, NoAssign, NoMove
    {
      public:
        explicit Thread(const std::string &name);
        virtual void Run() = 0;
        void Start();
        void Join();
      private:
        const std::string m_name;
        pthread_t m_thread;
    };
}
