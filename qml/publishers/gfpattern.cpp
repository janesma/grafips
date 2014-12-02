#include "gfpattern.h"

using namespace Grafips;

Pattern::Pattern(const QString &filter)
{
}

bool
Pattern::Matches(const std::string &path) const
{
    return false;
}

PatternSet::PatternSet(const QList<QString> &filters)
{
}

bool
PatternSet::Matches(const std::string &path) const
{
    for (std::vector<Pattern *>::const_iterator i = m_patterns.begin();
         i != m_patterns.end(); ++i)
    {
        if ((*i)->Matches(path))
            return true;
    }
    return false;
}

