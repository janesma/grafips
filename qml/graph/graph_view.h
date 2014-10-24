#pragma once

#include "graph_set.h"

// renders graph to texture for on-screen display
class GraphView
{
  public:
    GraphView();
    void Render();

  private:
    void CheckError(const char * file, int line);
    void PrintCompileError(GLint shader);

    GraphSet m_set;
};
