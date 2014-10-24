#pragma once

class DataSink
{
  public:
    virtual ~DataSink() {}
    virtual void data(int time, float val) = 0;
};


