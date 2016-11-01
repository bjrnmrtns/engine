#ifndef RENDER_TOOLS_H
#define RENDER_TOOLS_H

#include <glad/glad.h>

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

#endif
