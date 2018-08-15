#include "RenderMesh.hpp"
#include "RenderTools.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


namespace Render {
    namespace Mesh {
            Source::Source() {
                glGenBuffers(1, &vbo);
                glGenVertexArrays(1, &vao);
                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(::Mesh::Vertex), (void *) 0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(::Mesh::Vertex), (void *) (3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(::Mesh::Vertex), (void *) (6 * sizeof(float)));
                glEnableVertexAttribArray(2);
            }

            void Source::BufferData(const ::Mesh::Vertex data[], int elements) {
                this->elements = elements;
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(::Mesh::Vertex) * elements, &data[0], GL_STATIC_DRAW);
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
                        "layout (location = 0) in vec3 position;\n"
                        "layout (location = 1) in vec3 normal;\n"
                        "layout (location = 2) in vec3 color;\n"
                        "out vec3 ourColor;\n"
                        "uniform mat4 model;\n"
                        "uniform mat4 view;\n"
                        "uniform mat4 projection;\n"
                        "uniform float selectioncolor;\n"
                        "void main()\n"
                        "{\n"
                        "  gl_Position = projection * view * model * vec4(position, 1.0f);\n"
                        "  ourColor = color * selectioncolor;\n"
                        "}\n";

        static char const *fss =
                "#version 330 core\n"
                        "in vec3 ourColor;\n"
                        "out vec4 color;\n"
                        "void main()\n"
                        "{\n"
                        "  color = vec4(ourColor, 1.0);\n"
                        "}\n";

            Renderer::Renderer()
                    : shaderProgramId(CompileLinkVertexFragmentShader(vss, fss)),
                      modellocation(glGetUniformLocation(shaderProgramId, "model")),
                      viewlocation(glGetUniformLocation(shaderProgramId, "view")),
                      projectionlocation(glGetUniformLocation(shaderProgramId, "projection")),
                      selectioncolorlocation(glGetUniformLocation(shaderProgramId, "selectioncolor")){
            }

            Renderer::~Renderer() {
                glDeleteProgram(shaderProgramId);
            }

            void
            Renderer::Draw(Source &source, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, float selectioncolor) const {
                glUseProgram(shaderProgramId);
                glUniformMatrix4fv(modellocation, 1, false, glm::value_ptr(model));
                glUniformMatrix4fv(viewlocation, 1, false, glm::value_ptr(view));
                glUniformMatrix4fv(projectionlocation, 1, false, glm::value_ptr(projection));
                glUniform1f(selectioncolorlocation, selectioncolor);
                source.Draw();
            }
    }
}
