#include "cpu_provider.h"

#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <thread>

#include "publisher.h"

static const int READ_BUF_SIZE = 4096;

CpuProvider::CpuProvider(Publisher *p) : m_publisher(p)
{
    m_cpu_info_handle = open("/proc/stat", O_RDONLY);
    m_buf.resize(READ_BUF_SIZE);
    Refresh();
    p->RegisterProvider(this);
}

CpuProvider::~CpuProvider()
{
    close(m_cpu_info_handle);
}

struct CpuLine
{
    int user;
    int system;
    int idle;
};

void
CpuProvider::Refresh()
{
    // seek will refresh the data in the proc file
    lseek(m_cpu_info_handle, 0, SEEK_SET);

    const ssize_t bytes = read(m_cpu_info_handle, m_buf.data(), m_buf.size());
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
            ParseCpuLine(&m_systemStats, &wordsaveptr);
        }
        else
        {
            int core = atoi(&(word[3]));
            if (core >= m_core_stats.size())
                m_core_stats.resize(core + 1);
            ParseCpuLine(&(m_core_stats[core]), &wordsaveptr);
        }

        // advance to next line
        line = strtok_r(NULL, "\n", &linesaveptr);
    }
}


void 
CpuProvider::ParseCpuLine(CpuLine *dest, char **savePtr)
{
    static const int COUNT_STAT_ITEMS = 10;
    CpuLine current;
    int *current_item = &(current.user);

    for (int i = 0; i < COUNT_STAT_ITEMS; ++i,++current_item)
    {
        const char *num = strtok_r(NULL, " ", savePtr);
        assert(num != NULL);
        *current_item = atoi(num);
    }
    
    CpuLine delta;
    int *prev_item = &(dest->user);
    int *delta_item = &(delta.user);
    current_item = &(current.user);

    for (int i = 0; i < COUNT_STAT_ITEMS; ++i,++current_item, ++prev_item, ++delta_item)
    {
        *delta_item = *current_item - *prev_item;
    }

    const float active = delta.user + delta.nice + delta.system + delta.irq + delta.softirq + delta.guest + delta.guest_nice;
    const float total = active + delta.idle + delta.iowait + delta.steal;
    current.utilization = active / total;

    memcpy(dest, &current, sizeof(CpuLine));
}

bool
CpuProvider::IsEnabled() const
{
    return ! m_enabled_cores.empty();
}

void 
CpuProvider::GetDescriptions(std::vector<MetricDescription> *descriptions)
{
    descriptions->push_back(MetricDescription("/cpu/system/utilization", 
                                              "Displays percent cpu activity for the system",
                                              "CPU Busy", GR_METRIC_PERCENT));
    m_sysId = descriptions->back().id();

    for (int i = 0; i < m_core_stats.size(); ++i)
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
CpuProvider::Enable(int id)
{
    if (id == m_sysId)
    {
        m_enabled_cores.insert(-1);
        return;
    }
    for (int i = 0; i < m_ids.size(); ++i)
    {
        if (m_ids[i] == id)
        {
            m_enabled_cores.insert(i);
            return;
        }
    }
    assert(false);
}

void 
CpuProvider::Disable(int id)
{
    if (id == m_sysId)
    {
        m_enabled_cores.erase(-1);
        return;
    }
    for (int i = 0; i < m_ids.size(); ++i)
    {
        if (m_ids[i] == id)
        {
            m_enabled_cores.erase(i);
            return;
        }
    }
    assert(false);
}

void 
CpuProvider::Poll()
{
    if (! IsEnabled())
        return;

    Refresh();
    Publish();
}

void 
CpuProvider::Publish()
{
    DataSet d;
    const int ms = get_ms_time();

    if (m_enabled_cores.count(-1) != 0)
        d.push_back(DataPoint(ms, m_sysId, m_systemStats.utilization));

    for (int i = 0; i < m_ids.size(); ++i)
    {
        if (m_enabled_cores.count(i) == 0)
            continue;
        d.push_back(DataPoint(ms, m_ids[i], m_core_stats[i].utilization));
    }
    m_publisher->OnMetric(d);
}


void CpuProvider::Run()
{
    while (true)
    {
        Poll();
        usleep(1000000);
    }
}
