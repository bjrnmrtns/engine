#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderTools.hpp"
#include "RenderUI.hpp"
#include "VertexDefinitions.hpp"

namespace Render {
namespace UI {
Source::Source() {
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(::UI::Vertex),
                        (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(::UI::Vertex),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void Source::BufferData(const ::UI::Vertex data[], int elements) {
  this->elements = elements;
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(::UI::Vertex) * elements, &data[0],
               GL_STATIC_DRAW);
}

Source::~Source() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void Source::Draw() {
  assert(elements != -1);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, elements);
  glBindVertexArray(0);
}

static char const *vss =
    "#version 330 core\n"
    "layout (location = 0) in vec2 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 ourColor;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = projection * vec4(position, 0.0f, 1.0f);\n"
    "  ourColor = color;\n"
    "}\n";

static char const *fss = "#version 330 core\n"
                         "in vec3 ourColor;\n"
                         "out vec4 color;\n"
                         "void main()\n"
                         "{\n"
                         "  color = vec4(ourColor, 1.0f);\n"
                         "}\n";

Renderer::Renderer()
    : shaderProgramId(CompileLinkVertexFragmentShader(vss, fss)),
      projectionlocation(glGetUniformLocation(shaderProgramId, "projection")) {}

void Renderer::Draw(Source &source, const glm::mat4 &projection) const {
  glUseProgram(shaderProgramId);
  glUniformMatrix4fv(projectionlocation, 1, false, glm::value_ptr(projection));
  source.Draw();
}
} // namespace UI
} // namespace Render
