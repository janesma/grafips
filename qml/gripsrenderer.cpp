#include "gripsrenderer.h"

#include <QtOpenGL>

class PrivateRenderer : public QQuickFramebufferObject::Renderer
{
public:
    PrivateRenderer()
    {
    }

    void render() {
        glClearColor(0, 0,0,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0,1,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        update();
    }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

    //LogoRenderer logo;
};

QQuickFramebufferObject::Renderer *
GripsRenderer::createRenderer() const
{
    return new PrivateRenderer();
}
