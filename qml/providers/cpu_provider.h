#pragma once

#include <thread>
#include <set>
#include <mutex>

#include <QObject> 

#include "provider.h"
#include "publisher.h"

class CpuProvider : public QObject, public Provider
{
    Q_OBJECT
  public:
    Q_PROPERTY(Publisher *publisher READ publisher WRITE setPublisher NOTIFY onPublisher)
    Q_INVOKABLE void start();

    CpuProvider();
    ~CpuProvider();
    void GetDescriptions(std::vector<MetricDescription> *descriptions);
    void Enable(int id);
    void Disable(int id);
    void Poll();
    void Run();

    friend class CpuProviderFixture;

    Publisher *publisher() { return m_publisher; }
    void setPublisher(Publisher *p);
  signals:
    void onPublisher();

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

    std::thread *m_thread;
    std::mutex m_protect;
};
