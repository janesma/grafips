#pragma once

#include <vector>

enum MetricType
{
    GR_METRIC_COUNT,
    GR_METRIC_RATE,
    GR_METRIC_PERCENT
};

class MetricDescription
{
    MetricDescription(const MetricDescription &o);
    MetricDescription(int _id,     
                      const std::string &_path,
                      const std::string &_helpText,
                      const std::string &_displayName);
    const int id;
    const std::string path;
    const std::string helpText;
    const std::string displayName;
    MetricType type;
};

class Provider
{
  public:
    virtual ~Provider() {}
    virtual void getDescriptions(std::vector<MetricDescription> *descriptions) = 0;
    virtual void enable(int id) = 0;
    virtual void disable(int id) = 0;
};
