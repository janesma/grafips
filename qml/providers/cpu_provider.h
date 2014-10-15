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
    void getDescriptions(std::vector<MetricDescription> *descriptions);
    void enable(int id);
    void disable(int id);
    void poll();
    void run();

    friend class CpuProviderFixture;
  private:
    struct CpuLine
    {
        int user;
        int system;
        int idle;
        float utilization;
    };

    bool isEnabled() const;
    void refresh();
    void parseCpuLine(CpuLine *dest, char **savePtr);
    void publish();

    // file handle for /proc/stat
    int m_cpuInfoHandle;

    // thread that polls the file handle
    std::thread m_thread;

    // data structures to store the parsed line
    CpuLine m_systemStats;
    std::vector<CpuLine> m_coreStats;

    // stable buffer for reading
    std::vector<char> m_buf;

    // receives updates
    Publisher *m_publisher;

    // tracks subscriptions
    std::set<int> m_enabledCores;

    // translates metric ids to offsets
    int m_sysId;
    std::vector<int> m_ids;
};
