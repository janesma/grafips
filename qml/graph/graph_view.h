#pragma once

#include <map>
#include <QtQuick/QQuickFramebufferObject>

#include "graph_set.h"
#include "subscriber.h"
#include "publisher.h"

namespace Grafips
{
    class GraphView;

// renders graph to texture for on-screen display
    class GraphViewRenderer : public QQuickFramebufferObject::Renderer
    {
      public:
        GraphViewRenderer(GraphSetSubscriber *s, const Publisher &p);
        ~GraphViewRenderer();
        void render();
        void synchronize(QQuickFramebufferObject * item);
      private:
        void CheckError(const char * file, int line);
        void PrintCompileError(GLint shader);

        GraphSetSubscriber *m_subscriber;
        std::map<int, GraphSet *> m_sets;
        static const char *vshader;
        static const char *fshader;

        GLuint vbo;
        GLint attribute_coord2d, uniform_time, prog;
    };

    class GraphView : public QQuickFramebufferObject
    {
        Q_OBJECT
        Q_PROPERTY(Grafips::GraphSetSubscriber* subscriber READ subscriber WRITE setSubscriber)
        Q_PROPERTY(Grafips::Publisher* publisher READ publisher WRITE setPublisher)
      public:
        GraphView();
        ~GraphView();

        GraphSetSubscriber *subscriber() {return m_subscriber;}
        void setSubscriber(GraphSetSubscriber *s) {m_subscriber = s;}
        Publisher *publisher() {return m_pub;}
        void setPublisher(Publisher *p) {m_pub = p;}
    
        Renderer *createRenderer() const;

      private:
        Publisher *m_pub;
        GraphSetSubscriber *m_subscriber;
    };
}
