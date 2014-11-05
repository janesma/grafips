#pragma once
#include <map>
#include <vector>
#include <mutex>
#include <GLES2/gl2.h>

// interpolates data, provides it to view
class DataPoint;

class GraphSet
{
  public:
    struct Point {
        GLfloat x;
        GLfloat y;
    };
    typedef std::vector<Point> PointVec;

    GraphSet();
    void Add(const DataPoint &d);
    void SetLimit(int max_data_age);
    void GetData(PointVec *data);

  private:
    std::mutex m_protect;
    std::map<int, float> m_data;
    unsigned int m_current_time;
    int m_max_data_age;
};

