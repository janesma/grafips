#pragma once

#include <vector>
#include <string>

#include "metric.h"

class Provider
{
  public:
    virtual ~Provider() {}
    virtual void GetDescriptions(std::vector<MetricDescription> *descriptions) = 0;
    virtual void Enable(int id) = 0;
    virtual void Disable(int id) = 0;
    virtual void Poll() = 0;
};
