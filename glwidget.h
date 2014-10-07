#include <QtOpenGL>

class GLPlot : public QGLWidget
{
    Q_OBJECT
  public:
    explicit GLPlot(QWidget *p);
  protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

};

