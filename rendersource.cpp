#include "rendersource.h"
#include "glad/glad.h"

RenderSource::RenderSource(const int sizes[], int size)
{
    int stride = 0;
    for(int i = 0; i < size; i++)
    {
        stride += sizes[i];
    }
    this->stride = stride;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    intptr_t offset = 0;
    for(int i = 0; i < size; i++)
    {
        glVertexAttribPointer(i, sizes[i], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)offset);
        glEnableVertexAttribArray(i);
        offset += sizes[i] * sizeof(float);
    }
}
void RenderSource::Draw()
{
    assert(elements != -1);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, elements);
    glBindVertexArray(0);
}
RenderSource::~RenderSource()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
