#include "provider.h"

MetricDescription::MetricDescription(const MetricDescription &o) 
    : path(o.path), helpText(o.helpText), 
      displayName(o.displayName), type(o.type)
{}

MetricDescription::MetricDescription(const std::string &_path,
                                     const std::string &_helpText,
                                     const std::string &_displayName,
                                     MetricType _type)
    : path(_path), helpText(_helpText),
      displayName(_displayName), type(_type)
{}

int 
MetricDescription::id() const
{
    // hash the path
    int hash = 0;
    for (int i = 0; i < path.length(); ++i)
    {
        const char c = path[i];
        hash = 31 * hash + c;
    }
    return hash;
}
