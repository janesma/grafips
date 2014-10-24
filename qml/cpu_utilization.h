#pragma once

#include <thread>

class GripsRenderer;

class CpuUtilization
{
  public:
    CpuUtilization(GripsRenderer * r, int interval);
    ~CpuUtilization();
    void start();
    void join();
  private:
    void run();

    const int m_proc_stat_fh;
    const int m_interval;
    std::thread *m_thread;
    GripsRenderer *m_renderer;
};
