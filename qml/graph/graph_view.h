#pragma once

#include <QtQuick/QQuickFramebufferObject>

#include "graph_set.h"
#include "subscriber.h"
#include "publisher.h"

class GraphView;

// renders graph to texture for on-screen display
class GraphViewRenderer : public QQuickFramebufferObject::Renderer
{
  public:
    GraphViewRenderer(GraphSetSubscriber *s);
    void render();
    void synchronize(QQuickFramebufferObject * item);
    void AddMetric(int id);
    void AddPublisher(Publisher *p) { p->Subscribe(m_subscriber); }
  private:
    void CheckError(const char * file, int line);
    void PrintCompileError(GLint shader);

    GraphSetSubscriber *m_subscriber;
    GraphSet *m_set;
    static const char *vshader;
    static const char *fshader;

    GLuint vbo;
    GLint attribute_coord2d, uniform_time, prog;
};

class GraphView : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(GraphSetSubscriber* subscriber READ subscriber WRITE setSubscriber)
  public:
    GraphView();
    GraphSetSubscriber *subscriber()
        {
            return m_subscriber;
        }
    void setSubscriber(GraphSetSubscriber *s) 
        {
            m_subscriber = s;
        }
    //void SetSubscriber(GraphSetSubscriber *s);
    Renderer *createRenderer() const;
    void AddPublisher(Publisher* p) ;
    void AddMetric(int id);

    GraphSetSubscriber *m_subscriber;
    int m_id;
    Publisher *m_pub;
  private:
    static GraphViewRenderer *m_renderer;
};
