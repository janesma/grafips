#pragma once

#include <vector>
#include <string>

#include "metric.h"

class Provider
{
  public:
    virtual ~Provider() {}
    virtual void getDescriptions(std::vector<MetricDescription> *descriptions) = 0;
    virtual void enable(int id) = 0;
    virtual void disable(int id) = 0;
    virtual void poll() = 0;
};
