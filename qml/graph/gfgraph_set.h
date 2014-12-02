#pragma once
#include <map>
#include <vector>
#include <mutex>
#include <GLES2/gl2.h>

#include "gftraits.h"

namespace Grafips
{

// interpolates data, provides it to view
    class DataPoint;

    class GraphSet : NoCopy, NoAssign, NoMove
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
        void SetWidth(int w) { m_width = w; }
        void GetData(PointVec *data, unsigned int request_time_ms);
        void Clear();

      private:
        std::mutex m_protect;
        std::map<unsigned int, float> m_data;
        int m_max_data_age, m_width, m_time_correction;
    };

}
