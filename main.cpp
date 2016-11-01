#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>

#define GLM_FORCE_RADIANS 1
#include <SDL2/SDL.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "font.h"
#include "camera.h"
#include "shaderprogram.h"
#include "ui.h"
#include "rendersource.h"
#include "MeshRenderer.h"

static const bool SCREEN_FULLSCREEN = true;
static const int SCREEN_WIDTH  = 960;
static const int SCREEN_HEIGHT = 540;
static SDL_Window *window = NULL;
static SDL_GLContext maincontext;

static const char* uiVertexShaderSource =
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

static const char* uiFragmentShaderSource =
"#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"  color = vec4(ourColor, 1.0f);\n"
"}\n";

static const char* fontVertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec2 position;\n"
"layout (location = 1) in vec3 color;\n"
"layout (location = 2) in vec2 coord;\n"
"out vec3 ourColor;\n"
"out vec2 ourCoord;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"  gl_Position = projection * vec4(position, 0.0f, 1.0f);\n"
"  ourColor = color;\n"
"  ourCoord = coord;\n"
"}\n";

static const char* fontFragmentShaderSource =
"#version 330 core\n"
"in vec3 ourColor;\n"
"in vec2 ourCoord;\n"
"uniform sampler2D tex;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"  color = vec4(texture(tex, ourCoord).r, 0.0f, 0.0f, 1.0f);\n"
//"  color = vec4(ourColor, 1.0f);\n"
"}\n";

struct uivertex
{
    uivertex(glm::vec2 pos, glm::vec3 color)
    : pos(pos)
    , color(color)
    {
    }
    glm::vec2 pos;
    glm::vec3 color;
};

void drawRect(std::vector<uivertex>& buffer, int x, int y, int w, int h, glm::vec3 color)
{
    buffer.push_back(uivertex(glm::vec2(x + w, y + h), color));
    buffer.push_back(uivertex(glm::vec2(x, y + h), color));
    buffer.push_back(uivertex(glm::vec2(x, y), color));
    buffer.push_back(uivertex(glm::vec2(x + w, y + h), color));
    buffer.push_back(uivertex(glm::vec2(x, y), color));
    buffer.push_back(uivertex(glm::vec2(x + w, y), color));
}

void drawRectText(std::vector<textvertex>& buffer, int x, int y, int w, int h, glm::vec3 color)
{
    buffer.push_back(textvertex(glm::vec2(x + w, y + h), color, glm::vec2(1.0f, 1.0f)));
    buffer.push_back(textvertex(glm::vec2(x, y + h), color, glm::vec2(0.0f, 1.0f)));
    buffer.push_back(textvertex(glm::vec2(x, y), color, glm::vec2(0.0f, 0.0f)));
    buffer.push_back(textvertex(glm::vec2(x + w, y + h), color, glm::vec2(1.0f, 1.0f)));
    buffer.push_back(textvertex(glm::vec2(x, y), color, glm::vec2(0.0f, 0.0f)));
    buffer.push_back(textvertex(glm::vec2(x + w, y), color, glm::vec2(1.0f, 0.0f)));
}

int main()
{
	SDL_Event event;
	bool quit = false;
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Could not initialize SDL: %s\n",  SDL_GetError());
		exit(2);
	}
	atexit(SDL_Quit);
	SDL_GL_LoadLibrary(NULL);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	if(SCREEN_FULLSCREEN)
	{
		window = SDL_CreateWindow("demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL);
	}
	else
	{
		window = SDL_CreateWindow("demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	}

	maincontext = SDL_GL_CreateContext(window);
	if(maincontext == NULL)
	{
		fprintf(stderr, "Failed to create OpenGL context: %s\n",  SDL_GetError());
		exit(2);
	}
	// Check OpenGL properties
	printf("OpenGL loaded\n");
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));

	SDL_GL_SetSwapInterval(1);

	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);

    float cubeData[] {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, // Bottom-left
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // bottom-right
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // top-right
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, // bottom-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // bottom-left
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // top-right
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // bottom-right
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, // bottom-right
        // Right face
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // top-left
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // top-right
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // bottom-right
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // bottom-right
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, // bottom-left
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // top-left
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // top-right
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // bottom-left
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // top-left
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // bottom-right
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // top-left
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // top-right
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // bottom-right
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // top-left
    };

    Mesh::Source cube;
    cube.BufferData(&cubeData[0], sizeof(cubeData));
    
    int uiRenderSourceSizes[] = {2, 3};
    RenderSource uiRenderSource(uiRenderSourceSizes, 2);
    ShaderProgram uiShaderProgram(uiVertexShaderSource, uiFragmentShaderSource);
    
    
    std::vector<textvertex> textbuf;
    GenerateText("Perde Kop", textbuf);
    int textRenderSourceSizes[] = {2, 3, 2};
    RenderSource textRenderSource(textRenderSourceSizes, 3);
    std::vector<textvertex> textquad;
    drawRectText(textquad, 0, 0, 800, 30, glm::vec3(0.0f, 1.0f, 0.0f));
    ShaderProgram textShaderProgram(fontVertexShaderSource, fontFragmentShaderSource);
    
    Camera camera;
    unsigned int lastTime = SDL_GetTicks();
    bool w = false;
    bool a = false;
    bool s = false;
    bool d = false;
    
    bool rtsmode = false;

    Mesh::Renderer meshRenderer;
    UIState uistate(width / 2, height / 2);
	while(!quit)
	{
        unsigned int current = SDL_GetTicks();
        unsigned int deltams = current - lastTime;
        lastTime = current;

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 3d Scene Rendering
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), current / 500.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(45.0f, width / (float)height, 0.1f, 100.0f);
        meshRenderer.Draw(cube, model, camera.GetViewMatrix(), projection);
        
        // UI Rendering
        std::vector<uivertex> uibuffer;
        drawRect(uibuffer, uistate.mousex, uistate.mousey, 10, 10, glm::vec3(0.0f, 0.0f, 0.0f));
        uiRenderSource.BufferData(&uibuffer[0], uibuffer.size() * sizeof(uivertex));
        uiShaderProgram.Use();
        glm::mat4 projectionui = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
        int projectionlocationui = glGetUniformLocation(uiShaderProgram.GetId(), "projection");
        glUniformMatrix4fv(projectionlocationui, 1, false, glm::value_ptr(projectionui));
        glDisable(GL_DEPTH_TEST);
        uiRenderSource.Draw();
        
        textRenderSource.BufferData(&textbuf[0], textbuf.size() * sizeof(textvertex));
        textShaderProgram.Use();
        glUniformMatrix4fv(projectionlocationui, 1, false, glm::value_ptr(projectionui));
        glDisable(GL_DEPTH_TEST);
        textRenderSource.Draw();
        
		SDL_GL_SwapWindow(window);
        SDL_WarpMouseInWindow(window, width / 2.0f, height / 2.0f);
        SDL_SetRelativeMouseMode(SDL_TRUE);
        if(w)
        {
            camera.ProcessKeyboard(FORWARD, deltams / 1000.0f);
        }
        if(s)
        {
            camera.ProcessKeyboard(BACKWARD, deltams / 1000.0f);
        }
        if(a)
        {
            camera.ProcessKeyboard(LEFT, deltams / 1000.0f);
        }
        if(d)
        {
            camera.ProcessKeyboard(RIGHT, deltams / 1000.0f);
        }
		while(SDL_PollEvent(&event))
		{
            switch(event.type)
            {
                case SDL_QUIT:
                {
                    quit = true;
                    break;
                }
                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                        {
                            quit = true;
                            break;
                        }
                        case SDLK_w:
                        {
                            w = true;
                            break;
                        }
                        case SDLK_s:
                        {
                            s = true;
                            break;
                        }
                        case SDLK_a:
                        {
                            a = true;
                            break;
                        }
                        case SDLK_d:
                        {
                            d = true;
                            break;
                        }
                        case SDLK_TAB:
                        {
                            rtsmode = !rtsmode;
                            break;
                        }
                    }
                    break;
                }
                case SDL_KEYUP:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_w:
                        {
                            w = false;
                            break;
                        }
                        case SDLK_s:
                        {
                            s = false;
                            break;
                        }
                        case SDLK_a:
                        {
                            a = false;
                            break;
                        }
                        case SDLK_d:
                        {
                            d = false;
                            break;
                        }
                    }
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    if(!rtsmode)
                    {
                        camera.ProcessMouseMovement(event.motion.xrel, event.motion.yrel);
                    }
                    else
                    {
                        uistate.mousex = std::max(0, std::min(width, uistate.mousex + event.motion.xrel));
                        uistate.mousey = std::max(0, std::min(height, uistate.mousey + event.motion.yrel));
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    if (event.button.button == 1) uistate.mousedown = true;
                    break;
                }
                case SDL_MOUSEBUTTONUP:
                {
                    if (event.button.button == 1) uistate.mousedown = false;
                    break;
                }
            }
		}
	}
	return 0;
}
