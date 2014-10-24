#pragma once

#include <map>
#include <vector>

#include "metric.h"

// handles publications, distributes metric data to associated GraphSet
class Subscriber
{
  public:
    virtual ~Subscriber() {}
    virtual void OnMetric(const DataSet &d) = 0;
};


class GraphSet;

class GraphSetSubscriber : public Subscriber
{
  public:
    void AddSet(int id, GraphSet *);
    void OnMetric(const DataSet &d);
  private:
    std::map<int, GraphSet *> m_dataSets;
};
