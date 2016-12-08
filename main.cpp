#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>

#define GLM_FORCE_RADIANS 1
#include <SDL2/SDL.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include <array>

#include "UiElements.h"
#include "RenderMesh.h"
#include "RenderUI.h"
#include "Model.h"
#include "Camera.h"
#include "Configuration.h"

static const bool SCREEN_FULLSCREEN = false;
static const int SCREEN_WIDTH  = 960;
static const int SCREEN_HEIGHT = 540;
static SDL_Window *window = NULL;
static SDL_GLContext maincontext;

// TODO: Should be moved to seperate file
static bool MousePickTo3d(const glm::mat4& view, glm::mat4& proj, int width, int height, int mousex, int mousey, glm::vec3& outValue)
{
    glm::vec3 p0 = glm::unProject(glm::vec3(mousex, mousey, -1.0f), view, proj, glm::vec4(0.0f, 0.0f, width, height));
    glm::vec3 p1 = glm::unProject(glm::vec3(mousex, mousey, 1.0f), view, proj, glm::vec4(0.0f, 0.0f, width, height));
    glm::vec3 d = p1 - p0;
    float t = -p0.y / d.y;
    if (t > 0.0)
    {
        outValue = glm::vec3(p0.x + (d.x * t), 0, p0.z + (d.z * t));
        return true;
    }
    else
    {
        return false;
    }
}
struct Keys
{
    bool w = false, a = false, s = false, d = false; // camera movement
    bool leftctrl = false;
    bool t = false; // targetclick -> change to right mouse button
};

void HandleKeysForCamera(Keys& keys, Camera& camera)
{
    float movespeed = 0.5f;
    if(keys.w)
    {
        camera.Move(movespeed, 0.0f);
    }
    if(keys.s)
    {
        camera.Move(-movespeed, 0.0f);
    }
    if(keys.a)
    {
        camera.Move(0.0f, movespeed);
    }
    if(keys.d)
    {
        camera.Move(0.0f, -movespeed);
    }
}

struct Command
{
    enum CommandType { Tick, Move };
    int entityId;
    glm::vec3 targetPosition;
    CommandType type;
    int tick;
};

void SendToServer(int tick, std::vector<Command>& toSend, std::vector<Command>& serverCommandQueue)
{
    Command tickCommand;
    tickCommand.type = Command::Tick;
    tickCommand.tick = tick;
    serverCommandQueue.push_back(tickCommand);
    for(auto& command: toSend)
    {
        serverCommandQueue.push_back(command);
    }
    toSend.clear();
}

void ReceiveFromServer(std::vector<Command>& toReceive, std::vector<Command>& serverCommandQueue)
{
    for(auto& command: serverCommandQueue)
    {
        toReceive.push_back(command);
    }
    serverCommandQueue.clear();
}

struct Selection
{
    Selection() : state(NotSelecting) {};
    enum State { NotSelecting, BeginSelection, EndSelection };
    State state;
    //TODO: consider all 2d corners and convert all 4 to 3d and see what is selected
    // now the rectangle in 2d doesn't exactly represent the 3d one.
    glm::vec2 clicka;
    glm::vec2 clickb;
    glm::vec3 clicka3d;
    glm::vec3 clickb3d;
};

bool IsEntitySelected(const glm::vec3& cornerA, const glm::vec3& cornerB, const glm::vec3& entityPosition)
{
    float xmin = std::min(cornerA.x, cornerB.x);
    float zmin = std::min(cornerA.z, cornerB.z);
    float xmax = std::max(cornerA.x, cornerB.x);
    float zmax = std::max(cornerA.z, cornerB.z);
    return (  xmin < entityPosition.x && entityPosition.x < xmax
           && zmin < entityPosition.z && entityPosition.z < zmax);
}

class Entity
{
public:
    const int maxTeams = 1024;
    static int lastEntityId;
    Entity(const glm::vec3& startPosition)
    : entityId(lastEntityId++)
    , position(startPosition)
    {
    }
    int teamId = 0;
    int entityId;
    glm::vec3 position;
    bool selected = false;
    float speed = 5.0f; // 3 m/s
    bool isAtTarget = true;
    glm::vec3 targetPosition;
};
int Entity::lastEntityId = 0;

//TODO: Real networking (out of sync detection -> hash game state (placement new?))
int main(int argc, char* argv[])
{
    bool isServer = false;
    bool isClient = false;
    if(argc > 1 && (std::string(argv[1]) == std::string("--server")))
    {
        std::cout << "Starting as host..." << std::endl;
        isServer = true;
    }
    if(argc > 1 && (std::string(argv[1]) == std::string("--client")))
    {
        std::cout << "Starting as client..." << std::endl;
        isClient = true;
    }
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
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	SDL_GL_SetSwapInterval(1);

	int width, height;
	SDL_GetWindowSize(window, &width, &height);
    SDL_SetRelativeMouseMode(SDL_TRUE);
	glViewport(0, 0, width, height);

    float near = 0.1f;
    float far = 1000.0f;
    glm::mat4 projectionM = glm::perspective(45.0f, width / (float)height, near, far);

    Render::Mesh::Source grid;
    const ::Mesh::TStaticMesh gridData = Model::Grid();
    grid.BufferData((::Mesh::Vertex*)&gridData[0], gridData.size());

    Render::Mesh::Source tank;
    const ::Mesh::TStaticMesh tankData = Model::SimpleTank();
    tank.BufferData((::Mesh::Vertex*)&tankData[0], tankData.size());

    Render::UI::Source ui;

    Camera camera;
    Keys keys;

    Render::Mesh::Renderer meshRenderer;
    Render::UI::Renderer uiRenderer;
    int mousex = width / 2;
    int mousey = height / 2;
    Selection selection;
    std::vector<Entity> entities;
    for(size_t i = 0; i < sizeof(Configuration::EntityDefinitions) / sizeof(Configuration::EntityDefinition); i++)
    {
        entities.push_back(Entity(Configuration::EntityDefinitions[i].startpos));
    }
    std::vector<Command> clientCommandQueueToSend;
    std::vector<Command> serverCommandQueue;
    std::vector<Command> clientCommandQueue;
    const int stepFrequency = 120;
    auto serverBeginTime = std::chrono::steady_clock::now();
    std::chrono::duration<int, std::ratio<1, stepFrequency>> tick(1);
    int lastClientTick = 0;
	while(!quit)
	{
        auto currentTime = std::chrono::steady_clock::now();
        int serverTick = (currentTime - serverBeginTime) / tick;
        
        SendToServer(serverTick, clientCommandQueueToSend, serverCommandQueue);
        ReceiveFromServer(clientCommandQueue, serverCommandQueue);

        for(auto& command: clientCommandQueue)
        {
            switch(command.type)
            {
                case Command::Tick:
                {
                    int ticksTodo = command.tick - lastClientTick;
                    for(int i = 0; i < ticksTodo; i++)
                    {
                        for(auto it = entities.begin(); it != entities.end(); ++it)
                        {
                            if(!it->isAtTarget)
                            {
                                it->position += glm::normalize(it->targetPosition - it->position) * (it->speed / stepFrequency);
                                if(glm::length(it->position - it->targetPosition) < 1.0f)
                                {
                                    it->isAtTarget = true;
                                }
                            }
                        }
                    }
                    lastClientTick = command.tick;
                    break;
                }
                case Command::Move:
                {
                    for(auto& entity: entities)
                    {
                        if(entity.entityId == command.entityId)
                        {
                            entity.targetPosition = command.targetPosition;
                            entity.isAtTarget = false;
                        }
                    }
                    break;
                }
            }
        }
        clientCommandQueue.clear();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 3D rendering
        meshRenderer.Draw(grid, glm::mat4(1.0f), camera.GetView(), projectionM, 1);
        for(auto it = entities.begin(); it != entities.end(); ++it)
        {
            if(it->selected)
            {
                meshRenderer.Draw(tank, glm::translate(glm::mat4(1.0f), it->position), camera.GetView(), projectionM, 1.0);
            }
            else
            {
                meshRenderer.Draw(tank, glm::translate(glm::mat4(1.0f), it->position), camera.GetView(), projectionM, 0.5);
            }
        }

        if(selection.state == Selection::State::EndSelection)
        {
            selection.state = Selection::State::NotSelecting;
        }
        // UI Rendering
        std::vector<UI::Vertex> uibuffer;
        if(selection.state == Selection::State::BeginSelection)
        {
            uibuffer = SelectionRectangle(selection.clicka, glm::vec2(mousex, mousey), glm::vec3(0.0f, 0.6f, 0.0f));
        }
        else
        {
            uibuffer = generateSquare(mousex - 3, mousey - 3, 6, 6, glm::vec3(0.0f, 0.0f, 0.0f));
        }
        ui.BufferData(&uibuffer[0], uibuffer.size());
        glm::mat4 projectionui = glm::ortho(0.0f, (float)width, 0.0f, float(height), -1.0f, 1.0f);
        glDisable(GL_DEPTH_TEST);
        uiRenderer.Draw(ui, projectionui);
        
		SDL_GL_SwapWindow(window);
        HandleKeysForCamera(keys, camera);
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
                            keys.w = true;
                            break;
                        }
                        case SDLK_a:
                        {
                            keys.a = true;
                            break;
                        }
                        case SDLK_s:
                        {
                            keys.s = true;
                            break;
                        }
                        case SDLK_d:
                        {
                            keys.d = true;
                            break;
                        }
                        case SDLK_t:
                        {
                            keys.t = true;
                            break;
                        }
                        case SDLK_LCTRL:
                        {
                            keys.leftctrl = true;
                            break;
                        }
                        case SDLK_TAB:
                        {
                            static bool lasttab = true;
                            SDL_SetRelativeMouseMode(lasttab ? SDL_TRUE : SDL_FALSE);
                            lasttab = !lasttab;
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
                            keys.w = false;
                            break;
                        }
                        case SDLK_a:
                        {
                            keys.a = false;
                            break;
                        }
                        case SDLK_s:
                        {
                            keys.s = false;
                            break;
                        }
                        case SDLK_d:
                        {
                            keys.d = false;
                            break;
                        }
                        case SDLK_t:
                        {
                            keys.t = false;
                            break;
                        }
                        case SDLK_LCTRL:
                        {
                            keys.leftctrl = false;
                            break;
                        }
                    }
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    if(!keys.leftctrl) {
                        mousex = std::max(0, std::min(width, mousex + event.motion.xrel));
                        mousey = std::max(0, std::min(height, mousey - event.motion.yrel));
                    }
                    else {
                        camera.MouseMove(event.motion.xrel / 100.0f, -event.motion.yrel / 400.0f);
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    if(keys.t)
                    {
                        glm::vec3 target;
                        if(MousePickTo3d(camera.GetView(), projectionM, width, height, mousex, mousey, target))
                        {
                            for(auto it = entities.begin(); it != entities.end(); ++it)
                            {
                                if(it->selected)
                                {
                                    Command command;
                                    command.type = Command::Move;
                                    command.entityId = it->entityId;
                                    command.targetPosition = target;
                                    clientCommandQueueToSend.push_back(command);
                                }
                            }
                        }
                    }
                    if (!keys.t && selection.state == Selection::State::NotSelecting)
                    {
                        if(MousePickTo3d(camera.GetView(), projectionM, width, height, mousex, mousey, selection.clicka3d))
                        {
                            selection.state = Selection::State::BeginSelection;
                            selection.clicka = glm::vec2(mousex, mousey);
                        }
                    }
                    break;
                }
                case SDL_MOUSEBUTTONUP:
                {
                    if (selection.state == Selection::State::BeginSelection)
                    {
                        if(MousePickTo3d(camera.GetView(), projectionM, width, height, mousex, mousey, selection.clickb3d))
                        {
                            selection.state = Selection::State::EndSelection;
                            selection.clickb = glm::vec2(mousex, mousey);
                            for(auto it = entities.begin(); it != entities.end(); ++it)
                            {
                                if(IsEntitySelected(selection.clicka3d, selection.clickb3d, it->position))
                                {
                                    it->selected = true;
                                }
                                else
                                {
                                    it->selected = false;
                                }
                            }
                        }
                        else
                        {
                            selection.state = Selection::State::NotSelecting;
                        }
                    }
                    break;
                }
            }
		}
	}
	return 0;
}
