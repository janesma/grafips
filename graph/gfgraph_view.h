// Copyright (C) Intel Corp.  2014.  All Rights Reserved.

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice (including the
// next paragraph) shall be included in all copies or substantial
// portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//  **********************************************************************/
//  * Authors:
//  *   Mark Janes <mark.a.janes@intel.com>
//  **********************************************************************/

#ifndef GRAPH_GFGRAPH_VIEW_H_
#define GRAPH_GFGRAPH_VIEW_H_

#include <QtQuick/QQuickFramebufferObject>

#include <map>

#include "graph/gfgraph_set.h"
#include "subscriber/gfsubscriber.h"
#include "publishers/gfmetric_router.h"
#include "os/gftraits.h"

namespace Grafips {
class GraphView;

// renders graph to texture for on-screen display
class GraphViewRenderer : public QObject,
                          public QQuickFramebufferObject::Renderer,
                          NoCopy, NoAssign, NoMove {
  Q_OBJECT
 public:
  GraphViewRenderer(const GraphView *v, GraphSetSubscriber *s,
                    const PublisherInterface &p);
  ~GraphViewRenderer();
  void render();
  void synchronize(QQuickFramebufferObject * item);
  // to ensure that we get a multisample fbo
  QOpenGLFramebufferObject * createFramebufferObject(const QSize & size);

 signals:
  void maxChanged();

 private:
  void CheckError(const char * file, int line);
  void PrintCompileError(GLint shader);
  void RenderPoints(const GraphSet::PointVec &data, const float* color,
                    float max_y);
  void UpdateMax();

  GraphSetSubscriber *m_subscriber;
  std::map<int, GraphSet *> m_sets;
  static const char *vshader;
  static const char *fshader;

  GLuint vbo;
  GLint attribute_coord2d, uniform_x_range, uniform_max_y,
    uniform_line_color, prog;
  int m_width;
  GraphSet::PointVec m_data;
  float m_graph_max;
  std::map<int, float*> m_id_colors;
  bool m_stopped;
};

class GraphView : public QQuickFramebufferObject,
                  NoCopy, NoAssign, NoMove {
  Q_OBJECT
  Q_PROPERTY(Grafips::GraphSetSubscriber* subscriber
             READ subscriber)
  Q_PROPERTY(Grafips::MetricRouter* publisher
             READ publisher WRITE setPublisher)
  Q_PROPERTY(float graphMax
             READ graphMax WRITE setGraphMax NOTIFY onGraphMax)

 public:
  Q_INVOKABLE void setColor(int id, QString color);
  Q_INVOKABLE void stop();
  GraphView();
  ~GraphView();

  GraphSetSubscriber *subscriber() {return &m_subscriber;}
  MetricRouter *publisher() {return m_pub;}
  void setPublisher(MetricRouter *p) {m_pub = p;}
  Renderer *createRenderer() const;
  float graphMax() const { return m_graph_max;}
  void setGraphMax(float m);
  bool isStopped() { return m_stopped; }

  void GetColors(std::map<int, float*> *c);
 signals:
  void onGraphMax();

 private:
  MetricRouter *m_pub;
  GraphSetSubscriber m_subscriber;
  float m_graph_max;
  std::map<int, QString> m_colors;
  bool m_stopped;
};
}  // namespace Grafips

#endif  // GRAPH_GFGRAPH_VIEW_H_
