#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "Engine.h"
#include "Util/Timer.h"
#include "Util/Debug.h"

using namespace GameEngine;

Shader shader;
Model model;

Object object;

void Loop()
{
    glClearColor(0.20, 0.20, 0.25, 1.0);

    /*shader.Use();
    shader.SetMat4("projection", Renderer::Projection);
    shader.SetMat4("view", Renderer::View);

    glm::mat4 _model = glm::mat4(1.0f);
    _model = glm::translate(_model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    _model = glm::scale(_model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    shader.SetMat4("model", _model);
    model.Draw(shader);*/

    object._Update();
}

int main()
{
    Renderer::Init(800, 600, "Game Engine");
    Engine::Init();

    object.SetShader("Unlit");
    object.SetModel("Assets/Primitive/Cube.fbx");
    object._Start();
    
    shader.Init("Unlit.vert", "Unlit.frag");
    model.Init("Assets/Primitive/Plane.fbx");
    
    
    Engine::AddGameLoopCallback(Loop);
    Engine::StartGameLoop();
}
