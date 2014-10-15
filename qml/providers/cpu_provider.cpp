#include "cpu_provider.h"

#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static const int READ_BUF_SIZE = 4096;

CpuProvider::CpuProvider()
{
    m_cpuInfoHandle = open("/proc/stat", O_RDONLY);
    m_buf.resize(READ_BUF_SIZE);
    refresh();
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
    if (! isEnabled())
        return;

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
}

bool
CpuProvider::isEnabled() const
{
    return true;
}

void 
CpuProvider::getDescriptions(std::vector<MetricDescription> *descriptions)
{
}

void 
CpuProvider::enable(int id)
{
}
void 
CpuProvider::disable(int id)
{
}
