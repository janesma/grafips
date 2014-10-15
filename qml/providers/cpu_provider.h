#include <thread>
#include "provider.h"

class Publisher;

class CpuProvider : public Provider
{
  public:
    CpuProvider();
    ~CpuProvider();
    void getDescriptions(std::vector<MetricDescription> *descriptions);
    void enable(int id);
    void disable(int id);
    void run();

    friend class CpuProviderFixture;
  private:
    struct CpuLine
    {
        int user;
        int system;
        int idle;
    };

    bool isEnabled() const;
    void refresh();
    void parseCpuLine(CpuLine *dest, char **savePtr);

    int m_cpuInfoHandle;
    std::thread m_thread;
    CpuLine m_systemStats;
    std::vector<CpuLine> m_coreStats;
    std::vector<char> m_buf;
    Publisher *m_publisher;

};
