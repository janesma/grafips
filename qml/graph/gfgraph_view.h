#pragma once

#include <map>
#include <QtQuick/QQuickFramebufferObject>

#include "gfgraph_set.h"
#include "gfsubscriber.h"
#include "gfpublisher_remote.h"
#include "gftraits.h"

namespace Grafips
{
    class GraphView;

// renders graph to texture for on-screen display
    class GraphViewRenderer : public QQuickFramebufferObject::Renderer,
                              NoCopy, NoAssign, NoMove
    {
      public:
        GraphViewRenderer(GraphSetSubscriber *s, const PublisherInterface &p);
        ~GraphViewRenderer();
        void render();
        void synchronize(QQuickFramebufferObject * item);
        // to ensure that we get a multisample fbo
        QOpenGLFramebufferObject * createFramebufferObject(const QSize & size);
      private:
        void CheckError(const char * file, int line);
        void PrintCompileError(GLint shader);
        void RenderPoints(const GraphSet::PointVec &data, const float* color);

        GraphSetSubscriber *m_subscriber;
        std::map<int, GraphSet *> m_sets;
        static const char *vshader;
        static const char *fshader;

        GLuint vbo;
        GLint attribute_coord2d, uniform_time, uniform_line_color, prog;
        int m_width;
    };

    class GraphView : public QQuickFramebufferObject,
                      NoCopy, NoAssign, NoMove

    {
        Q_OBJECT
        Q_PROPERTY(Grafips::GraphSetSubscriber* subscriber READ subscriber WRITE setSubscriber)
        Q_PROPERTY(Grafips::PublisherStub* publisher READ publisher WRITE setPublisher)
      public:
        GraphView();
        ~GraphView();

        GraphSetSubscriber *subscriber() {return m_subscriber;}
        void setSubscriber(GraphSetSubscriber *s) {m_subscriber = s;}
        PublisherStub *publisher() {return m_pub;}
        void setPublisher(PublisherStub *p) {m_pub = p;}
    
        Renderer *createRenderer() const;

      private:
        PublisherStub *m_pub;
        GraphSetSubscriber *m_subscriber;
    };
}
