#pragma once

#include <vector>

class Provider;

struct DataPoint
{
    DataPoint(int t, int i, float d) : timeVal(t), id(i), data(d) {}
    int   timeVal;
    int   id;
    float data;
};

typedef std::vector<DataPoint> DataSet;
typedef std::vector<MetricDescription> MetricDescriptionSet;

class Publisher
{
  public:
    virtual ~Publisher() {}
    virtual void registerProvider(Provider *p) = 0;
    virtual void onMetric(const DataSet &d) = 0;
    virtual void enable(int id) = 0;
    virtual void getDescriptions(std::vector<MetricDescription> *descriptions) = 0;
};
