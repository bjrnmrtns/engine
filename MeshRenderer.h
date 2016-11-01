#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace {

//TODO: This should go to a renderer utility file.
int CompileLinkVertexFragmentShader(const char* vss, const char* fss)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vss, NULL);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char shaderCompileInfoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, shaderCompileInfoLog);
        std::cout << "Vertex shader compilation failed: " << shaderCompileInfoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fss, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char shaderCompileInfoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, shaderCompileInfoLog);
        std::cout << "Fragment shader compilation failed: " << shaderCompileInfoLog << std::endl;
    }

    unsigned int id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success)
    {
        char shaderLinkInfoLog[512];
        glGetProgramInfoLog(id, 512, NULL, shaderLinkInfoLog);
        std::cout << "Linking shader program failed: " << shaderLinkInfoLog << std::endl;
    }
    return id;
}
}

namespace Mesh
{
struct Vertex
{
	glm::vec3 pos;
	glm::vec3 col;
};

class Source
{
public:
	Source()
	{
        glGenBuffers(1, &vbo);
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
	}
    template <typename T>
    void BufferData(const T& data, int size)
    {
        this->elements = size / stride;
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, size, &data[0], GL_STATIC_DRAW);
    }
    ~Source()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
    void Draw()
    {
        assert(elements != -1);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, elements);
        glBindVertexArray(0);
    }
    unsigned int vbo;
    unsigned int vao;
    int elements = -1;
    const int stride = 6;
};

// UIRenderer -> contains hardcoded ui shader + matrices. Can render multiple uiRenderSources/data
// MeshRenderer -> contains hardcoded mesh shader + matrices. Can render multiple meshes/data
// Apart from a Renderable we do not need any abstractions regarding, opengl. We can even hardcode
// glVertexAttribPointer
// Use MeshRenderer as test put helper functions and MeshRenderer in seperate file for rendering. RenderSource code needs to be integrated in MeshRenderer.


class Renderer
{
public:
	Renderer()
    : shaderProgramId(CompileLinkVertexFragmentShader(vss, fss))
    , modellocation(glGetUniformLocation(shaderProgramId, "model"))
    , viewlocation(glGetUniformLocation(shaderProgramId, "view"))
    , projectionlocation(glGetUniformLocation(shaderProgramId, "projection"))
    {
    }
    ~Renderer()
    {
        glDeleteProgram(shaderProgramId);
    }
    void Draw(Source& source, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
    {
    	glUseProgram(shaderProgramId);
        glUniformMatrix4fv(modellocation, 1, false, glm::value_ptr(model));
        glUniformMatrix4fv(viewlocation, 1, false, glm::value_ptr(view));
        glUniformMatrix4fv(projectionlocation, 1, false, glm::value_ptr(projection));
    	source.Draw();
    }
private:
    const int shaderProgramId;
    const int modellocation;
    const int viewlocation;
    const int projectionlocation;

static constexpr char* vss =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 ourColor;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"  gl_Position = projection * view * model * vec4(position, 1.0f);\n"
"  ourColor = color;\n"
"}\n";

static constexpr char* fss =
"#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"  color = vec4(ourColor, 1.0);\n"
"}\n";
};
}

#endif
