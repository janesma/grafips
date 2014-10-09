#include <QtWidgets>
#include <QtOpenGL>

#include "glwidget.h"
GLPlot::GLPlot(QWidget *p)
{
}

void
GLPlot::initializeGL()
{
    qglClearColor(Qt::blue);
    
}

void 
GLPlot::resizeGL(int w, int h)
{
}
void 
GLPlot::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

