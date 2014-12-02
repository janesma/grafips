#include "gfgraph_view.h"

#include <assert.h>

#include <QtOpenGL>
#include <GLES2/gl2.h>

using namespace Grafips;

#define GL_CHECK() CheckError(__FILE__, __LINE__)

const char *
GraphViewRenderer::vshader = "attribute vec2 coord2d;\n" 
    "uniform float time_offset;\n"
    "void main(void) {\n" 
    "  float x = coord2d.x - time_offset;"
    "  gl_Position = vec4(x, coord2d.y, 0, 1);\n" 
    "}";

const char *
GraphViewRenderer::fshader = "uniform vec4 line_color;"
    "void main(void) {"
    "   gl_FragColor = line_color;"
    "}";

float tick_lines_color[4] = { 0, 0, 0, .2 };
float black_color[4] = { 0, 0, 0, 1 };

GraphViewRenderer::GraphViewRenderer(GraphSetSubscriber *s,
                                     const PublisherInterface &p) : m_subscriber(s)
{
    glGenBuffers(1, &vbo);
    GL_CHECK();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
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

    uniform_line_color = glGetUniformLocation(prog, "line_color");
    GL_CHECK();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GL_CHECK();
}

QOpenGLFramebufferObject *
GraphViewRenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    //format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(20);
    return new QOpenGLFramebufferObject(size, format);
}

GraphViewRenderer::~GraphViewRenderer()
{
    while (!m_sets.empty())
    {
        m_subscriber->RemoveSet(m_sets.begin()->first);
        delete m_sets.begin()->second;
        m_sets.erase(m_sets.begin());
    }
}

void
GraphViewRenderer::RenderPoints(const GraphSet::PointVec &data, const float* color)
{
    if (data.empty() )
        return;

    // static int last_size = 0;
    // if (last_size != data.size())
    // {
    //     std::cout << "size: " << data.size() << std::endl;
    //     for (int i = 0; i < data.size(); ++i )
    //     {
    //         std::cout << data[i].x << "," << data[i].y << std::endl;
    //     }
    // }
    // last_size = data.size();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GL_CHECK();

    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GraphSet::Point), 
                 data.data(), GL_STATIC_DRAW);
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

    // GLint t = GetTimeOffset();
    // assert(t >= 0);
    // assert(t <2000);
    // float x_offset = t / 1000.0;
    // glUniform1f(uniform_time, x_offset);
    glUniform1f(uniform_time, 0.0);
    GL_CHECK();

    glUniform4f(uniform_line_color, color[0], color[1], color[2], color[3]);

    glDrawArrays(GL_LINE_STRIP, 0, data.size());
    GL_CHECK();

    // x_offset = -2.0 + (t / 1000.0);
    // printf("t:%i\tx:%f\ty:%f\tcount:%d\n", t, graph[t].x - x_offset, graph[t].y, t);
    // glUniform1f(uniform_time, x_offset);
    // GL_CHECK();
    // glDrawArrays(GL_LINE_STRIP, 0, t);
    // GL_CHECK();
 
    glDisableVertexAttribArray(attribute_coord2d);
    GL_CHECK();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GL_CHECK();
}

void 
GraphViewRenderer::render()
{
    glClearColor(1,1,1,1);
    GL_CHECK();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GL_CHECK();

    glUseProgram(prog);
    GL_CHECK();

    static GraphSet::PointVec data;

    glLineWidth(2);
    // render the cross-lines representing 25,50,75 %
    data.resize(2);
    data[0].x = -1; data[0].y = 0;
    data[1].x = 1; data[1].y = 0;
    RenderPoints(data, tick_lines_color);

    data[0].x = -1; data[0].y = .5;
    data[1].x = 1; data[1].y = .5;
    RenderPoints(data, tick_lines_color);

    data[0].x = -1; data[0].y = -.5;
    data[1].x = 1; data[1].y = -.5;
    RenderPoints(data, tick_lines_color);

    // render the per-10s vertical lines
    const unsigned int t = get_ms_time();
    const float offset_t = 1.0  - ((float)(t % 10000))/10000.0;
    const float line_distance = 2.0 * 10.0 / 60.0;
    data[0].x = -1 + offset_t * line_distance; data[0].y = -1;
    data[1].x = data[0].x; data[1].y = 1;
    while (data[0].x < 1.0)
    {
        // std::cout << data[0].x << ", " << data[0].y << " : ";
        // std::cout << data[1].x << ", " << data[1].y << std::endl;
        RenderPoints(data, tick_lines_color);
        data[0].x += line_distance;
        data[1].x = data[0].x;
    }

    glLineWidth(1);

    for (std::map<int, GraphSet *>::iterator set = m_sets.begin();
         set != m_sets.end(); ++set)
    {
        set->second->SetWidth(m_width);
        set->second->GetData(&data, t);

        RenderPoints(data, black_color);        

    }
    update();
}

void 
GraphViewRenderer::CheckError(const char * file, int line)
{
    if (glGetError() == GL_NO_ERROR)
        return;
    printf("ERROR: %s:%i\n", file, line);
    exit(-1);
}

void 
GraphViewRenderer::PrintCompileError(GLint shader)
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

void 
GraphViewRenderer::synchronize(QQuickFramebufferObject * item) 
{
    // call the subscriber to make a one-time request for the metrics to be graphed.
    if (! m_sets.empty())
        return;

    std::vector<int> ids;
    m_subscriber->GetIDs(&ids);

    for (std::vector<int>::iterator i = ids.begin();
         i != ids.end(); ++i)
    {
        const int id = *i;
        GraphSet *gs = new GraphSet();
        m_sets[id] = gs;
        m_subscriber->AddSet(id, gs);
    }
}

GraphView::GraphView() : m_pub(NULL),
                         m_subscriber(NULL)
                         
{
    setTextureFollowsItemSize(true);
}


QQuickFramebufferObject::Renderer *
GraphView::createRenderer() const 
{ 
    assert (m_pub);
    assert (m_subscriber);
    QQuickFramebufferObject::Renderer * renderer = new GraphViewRenderer(m_subscriber, *m_pub);
    return renderer;
}

GraphView::~GraphView()
{
}
