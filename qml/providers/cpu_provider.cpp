#include "cpu_provider.h"

#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

#include "publisher.h"

static const int READ_BUF_SIZE = 4096;

CpuProvider::CpuProvider(Publisher *p) : m_publisher(p)
{
    m_cpuInfoHandle = open("/proc/stat", O_RDONLY);
    m_buf.resize(READ_BUF_SIZE);
    refresh();
    p->registerProvider(this);
}

CpuProvider::~CpuProvider()
{
    close(m_cpuInfoHandle);
}

struct CpuLine
{
    int user;
    int system;
    int idle;
};

void
CpuProvider::refresh()
{
    // seek will refresh the data in the proc file
    lseek(m_cpuInfoHandle, 0, SEEK_SET);

    const ssize_t bytes = read(m_cpuInfoHandle, m_buf.data(), m_buf.size());
    assert(bytes > 0);
    m_buf[bytes] = '\0';

    char *linesaveptr, *wordsaveptr;
    char *line = strtok_r(m_buf.data(), "\n", &linesaveptr);
    assert(line != NULL);
    while (true)
    {
        // parse line
        char *word = strtok_r(line, " ", &wordsaveptr);
        assert(word != NULL);
        if (0 != strncmp("cpu", word, 3))
            // cpu lines expected at front of /proc/stat
            break;

        if (0 == strncmp("cpu", word, 5))
        {
            parseCpuLine(&m_systemStats, &wordsaveptr);
        }
        else
        {
            int core = atoi(&(word[3]));
            if (core >= m_coreStats.size())
                m_coreStats.resize(core + 1);
            parseCpuLine(&(m_coreStats[core]), &wordsaveptr);
        }

        // advance to next line
        line = strtok_r(NULL, "\n", &linesaveptr);
    }
}

void 
CpuProvider::parseCpuLine(CpuLine *dest, char **savePtr)
{
    const char *user = strtok_r(NULL, " ", savePtr);
    assert(user != NULL);
    dest->user = atoi(user);

    strtok_r(NULL, " ", savePtr); // skip nice

    const char *system = strtok_r(NULL, " ", savePtr);
    assert (system != NULL);
    dest->system = atoi(system);

    const char *idle = strtok_r(NULL, " ", savePtr);
    assert (idle != NULL);
    dest->idle = atoi(idle);

    const float active = dest->system + dest->user;
    const float total = active + dest->idle;
    dest->utilization = active / total;
}

bool
CpuProvider::isEnabled() const
{
    return ! m_enabledCores.empty();
}

void 
CpuProvider::getDescriptions(std::vector<MetricDescription> *descriptions)
{
    descriptions->push_back(MetricDescription("/cpu/system/utilization", 
                                              "Displays percent cpu activity for the system",
                                              "CPU Busy", GR_METRIC_PERCENT));
    m_sysId = descriptions->back().id();

    for (int i = 0; i < m_coreStats.size(); ++i)
    {
        std::stringstream s;
        s << "/cpu/core" << i << "/utilization";
        descriptions->push_back(MetricDescription(s.str(), 
                                                  "Displays percent cpu activity for the core",
                                                  "CPU Core Busy", GR_METRIC_PERCENT));
        if (m_ids.size() <= i)
            m_ids.push_back(descriptions->back().id());
    }
}

void 
CpuProvider::enable(int id)
{
    if (id == m_sysId)
    {
        m_enabledCores.insert(-1);
        return;
    }
    for (int i = 0; i < m_ids.size(); ++i)
    {
        if (m_ids[i] == id)
        {
            m_enabledCores.insert(i);
            return;
        }
    }
    assert(false);
}

void 
CpuProvider::disable(int id)
{
    if (id == m_sysId)
    {
        m_enabledCores.erase(-1);
        return;
    }
    for (int i = 0; i < m_ids.size(); ++i)
    {
        if (m_ids[i] == id)
        {
            m_enabledCores.erase(i);
            return;
        }
    }
    assert(false);
}

void 
CpuProvider::poll()
{
    if (! isEnabled())
        return;

    refresh();
    publish();
}

void 
CpuProvider::publish()
{
    DataSet d;
    const int ms = get_ms_time();

    if (m_enabledCores.count(-1) != 0)
        d.push_back(DataPoint(ms, m_sysId, m_systemStats.utilization));

    for (int i = 0; i < m_ids.size(); ++i)
    {
        if (m_enabledCores.count(i) == 0)
            continue;
        d.push_back(DataPoint(ms, m_ids[i], m_coreStats[i].utilization));
    }
    m_publisher->onMetric(d);
}
