#ifndef GRIPS_RENDERER_H
#define GRIPS_RENDERER_H

#include <mutex>
#include <QtQuick/QQuickFramebufferObject>

#include "data_sink.h"

class GripsRenderer : public QQuickFramebufferObject,
                      public DataSink
{
    Q_OBJECT
public:
    GripsRenderer()
        {
            setTextureFollowsItemSize(true);
        }
    Renderer *createRenderer() const;
    void data(int time, float val);
  private:
    std::mutex m_mut;
};

#endif
