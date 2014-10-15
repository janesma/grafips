#pragma once

#include <vector>

class Provider;

struct DataPoint
{
    DataPoint(int t, int i, float d) : time_val(t), id(i), data(d) {}
    int   time_val;
    int   id;
    float data;
};

typedef std::vector<DataPoint> DataSet;
typedef std::vector<MetricDescription> MetricDescriptionSet;

class Publisher
{
  public:
    virtual ~Publisher() {}
    virtual void RegisterProvider(Provider *p) = 0;
    virtual void OnMetric(const DataSet &d) = 0;
    virtual void Enable(int id) = 0;
    virtual void GetDescriptions(std::vector<MetricDescription> *descriptions) = 0;
};
