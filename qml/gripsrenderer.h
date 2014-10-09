#ifndef GRIPS_RENDERER_H
#define GRIPS_RENDERER_H

#include <QtQuick/QQuickFramebufferObject>

//class LogoRenderer;

class GripsRenderer : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    Renderer *createRenderer() const;
};

#endif
