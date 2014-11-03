#pragma once

#include <thread>
#include <set>
#include "provider.h"

class Publisher;

class CpuProvider : public Provider
{
  public:
    CpuProvider(Publisher *p);
    ~CpuProvider();
    void GetDescriptions(std::vector<MetricDescription> *descriptions);
    void Enable(int id);
    void Disable(int id);
    void Poll();
    void Run();

    friend class CpuProviderFixture;
  private:
    struct CpuLine
    {
        int user;
        int nice;
        int system;
        int idle;
        int iowait;
        int irq;
        int softirq;
        int steal;
        int guest;
        int guest_nice;
        float utilization;
    };

    bool IsEnabled() const;
    void Refresh();
    void ParseCpuLine(CpuLine *dest, char **savePtr);
    void Publish();

    // file handle for /proc/stat
    int m_cpu_info_handle;

    // thread that polls the file handle
    std::thread m_thread;

    // data structures to store the parsed line
    CpuLine m_systemStats;
    std::vector<CpuLine> m_core_stats;

    // stable buffer for reading
    std::vector<char> m_buf;

    // receives updates
    Publisher *m_publisher;

    // tracks subscriptions
    std::set<int> m_enabled_cores;

    // translates metric ids to offsets
    int m_sysId;
    std::vector<int> m_ids;
};
