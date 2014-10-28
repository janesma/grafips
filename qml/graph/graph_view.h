#pragma once

#include <QtQuick/QQuickFramebufferObject>

#include "graph_set.h"
#include "subscriber.h"
#include "publisher.h"

class GraphSetSubscriber;
class GraphView;

// renders graph to texture for on-screen display
class GraphViewRenderer : public QQuickFramebufferObject::Renderer
{
  public:
    GraphViewRenderer();
    void render();
    void synchronize(QQuickFramebufferObject * item);
    void AddMetric(int id);
    void AddPublisher(Publisher *p) { p->Subscribe(&m_subscriber); }
  private:
    void CheckError(const char * file, int line);
    void PrintCompileError(GLint shader);

    GraphSetSubscriber m_subscriber;
    GraphSet *m_set;
    static const char *vshader;
    static const char *fshader;

    GLuint vbo;
    GLint attribute_coord2d, uniform_time, prog;
};


class GraphView : public QQuickFramebufferObject
{
  public:
    GraphView() : m_id(-1), m_pub(NULL) 
        {setTextureFollowsItemSize(true);}
    QQuickFramebufferObject::Renderer *createRenderer() const 
    { 
        if (m_renderer)
            return m_renderer;
        m_renderer = new GraphViewRenderer();
        if (m_id != -1)
            m_renderer->AddMetric(m_id);
        if (m_pub)
            m_renderer->AddPublisher(m_pub); 
        return m_renderer;
    }

    
    void AddPublisher(Publisher* p) 
        { 
            m_pub = p;
        }
    void AddMetric(int id) 
        { 
            m_id = id;
        }
    int m_id;
    Publisher *m_pub;
  private:
    static GraphViewRenderer *m_renderer;
};
