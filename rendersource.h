#ifndef RENDERSOURCE_H
#define RENDERSOURCE_H

#include "glad/glad.h"
#include <vector>
#include <cassert>

class RenderSource
{
public:
    RenderSource(const int sizes[], int size);
    template <typename T>
    void BufferData(const T& data, int size)
    {
        this->elements = size / stride;
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, size, &data[0], GL_STATIC_DRAW);
    }
    ~RenderSource();
    void Draw();
    unsigned int vbo;
    unsigned int vao;
    int elements = -1;
    int stride;
};

#endif
