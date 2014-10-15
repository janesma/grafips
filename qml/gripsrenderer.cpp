#include "gripsrenderer.h"

#include <time.h>
#include <assert.h>

#include <QtOpenGL>
#include <GLES2/gl2.h>

struct point {
  GLfloat x;
  GLfloat y;
};

#define GL_CHECK() CheckError(__FILE__, __LINE__)

class PrivateRenderer : public QQuickFramebufferObject::Renderer
{
public:
    PrivateRenderer()
        {
 
            for(int i = 0; i < 2000; i++) {
                float x = (i - 1000.0) / 1000.0;
                graph[i].x = x;
                graph[i].y = sin(x * 10.0) / (1.0 + x * x);
                //printf ("x:%f\ty:%f\n", x, graph[i].y);
            }    
 
            glGenBuffers(1, &vbo);
            GL_CHECK();
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            GL_CHECK();
            glBufferData(GL_ARRAY_BUFFER, sizeof graph, graph, GL_STATIC_DRAW);
            GL_CHECK();

            const int vs = glCreateShader(GL_VERTEX_SHADER);
            GL_CHECK();
            int len = strlen(vshader);
            glShaderSource(vs, 1, &vshader, &len);
            GL_CHECK();
            glCompileShader(vs);
            PrintCompileError(vs);

            const int fs = glCreateShader(GL_FRAGMENT_SHADER);
            GL_CHECK();
            len = strlen(fshader);
            glShaderSource(fs, 1, &fshader, &len);
            GL_CHECK();
            glCompileShader(fs);
            PrintCompileError(fs);

            prog = glCreateProgram();
            glAttachShader(prog, vs);
            GL_CHECK();
            glAttachShader(prog, fs);
            GL_CHECK();
            glLinkProgram(prog);
            GL_CHECK();

            attribute_coord2d = glGetAttribLocation(prog,  "coord2d");
            GL_CHECK();
            uniform_time = glGetUniformLocation(prog, "time_offset");
            GL_CHECK();
        }

    GLint GetTimeOffset()
        {
            struct timespec tp;
            clock_gettime(CLOCK_REALTIME, &tp);
            GLint ms = tp.tv_nsec / 1000000;
            ms += (tp.tv_sec % 10) * 1000;  // 10 sec period
            return ms / 5; // 10000 ms / 5 -> 2000 data points
        }

    void render() 
        {
            glClearColor(1,1,1,1);
            GL_CHECK();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            GL_CHECK();
            glUseProgram(prog);
            GL_CHECK();
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            GL_CHECK();
 
            glEnableVertexAttribArray(attribute_coord2d);
            GL_CHECK();

            glVertexAttribPointer(attribute_coord2d,   // attribute
                                  2,                   // number of elements per vertex, here (x,y)
                                  GL_FLOAT,            // the type of each element
                                  GL_FALSE,            // take our values as-is
                                  0,                   // no space between values
                                  0                    // use the vertex buffer object
                );
            GL_CHECK();

            
            GLint t = GetTimeOffset();
            assert(t >= 0);
            assert(t <2000);
            float x_offset = t / 1000.0;
            glUniform1f(uniform_time, x_offset);
            GL_CHECK();

            glDrawArrays(GL_LINE_STRIP, t, 2000 - t);
            GL_CHECK();

            x_offset = -2.0 + (t / 1000.0);
            // printf("t:%i\tx:%f\ty:%f\tcount:%d\n", t, graph[t].x - x_offset, graph[t].y, t);
            glUniform1f(uniform_time, x_offset);
            GL_CHECK();
            glDrawArrays(GL_LINE_STRIP, 0, t);
            GL_CHECK();
 
            glDisableVertexAttribArray(attribute_coord2d);
            GL_CHECK();
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            GL_CHECK();


            update();
        }

    void PrintCompileError(GLint shader)
        {
            GLint status;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
            if (status == GL_TRUE)
                return;
            static const int MAXLEN = 1024;
            std::vector<char> log(MAXLEN);
            GLsizei len;
            glGetShaderInfoLog(shader,  MAXLEN,  &len, log.data()); 
            printf("ERROR -- compile failed: %s\n", log.data());
        }

    void CheckError(const char * file, int line)
        {
            if (glGetError() == GL_NO_ERROR)
                return;
            printf("ERROR: %s:%i\n");
            exit(-1);
        }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) {
        QOpenGLFramebufferObjectFormat format;
        //format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

    static const char *vshader;
    static const char *fshader;

    GLuint vbo;
    point graph[2000];
    GLint attribute_coord2d, uniform_time, prog;
};

QQuickFramebufferObject::Renderer *
GripsRenderer::createRenderer() const
{
    return new PrivateRenderer();
}

const char *
PrivateRenderer::vshader = "attribute vec2 coord2d;\n" 
    "uniform float time_offset;\n"
    "void main(void) {\n" 
    "  float x = coord2d.x - time_offset;"
    "  gl_Position = vec4(x, coord2d.y, 0, 1);\n" 
    "}";

const char *
PrivateRenderer::fshader = "void main(void) {"
    "  gl_FragColor = vec4(0,0,0,1);"
    "}";

void 
GripsRenderer::Data(int time, float val)
{
}
