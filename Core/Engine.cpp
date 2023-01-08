#include <glad/glad.h>
#include "Engine.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "EditorCamera.h"
#include "UI/UIManager.h"
#include "Util/Timer.h"

#include "Util/Entity.h"

using namespace GameEngine;

std::vector<std::function<void()>> Engine::m_GameLoops;
std::shared_ptr<std::vector<std::shared_ptr<Object>>> Engine::m_Objects = std::make_shared<std::vector<std::shared_ptr<
    Object>>>();
std::vector<std::shared_ptr<Shader>> Engine::m_Shaders;

Camera* Engine::m_CurrentCamera;

void Engine::Init()
{
    UIManager::Init();
}

std::shared_ptr<Object> Engine::CreateObject(std::string name, std::string shaderPath, std::string modelPath)
{
    auto _object = std::make_shared<Object>();
    _object->SetName(name);
    _object->SetInfo(shaderPath, modelPath);
    AddObject(_object);
    return _object;
}

void Engine::AddObject(std::shared_ptr<Object> object)
{
    m_Objects->emplace_back(std::move(object));
    object->_Start();
}

void GameEngine::Engine::StartGameLoop()
{
    glEnable(GL_DEPTH_TEST);

    Renderer::CreateFramebuffer();

    Camera camera;
    SetCurrentCamera(&camera);
    
    while (!glfwWindowShouldClose(Renderer::GetWindow()))
    {
        Timer timer;

        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
        
        Renderer::UpdateDeltaTime();
        camera.HandleInput();
        if (Renderer::RenderSize.x >= 1 && Renderer::RenderSize.y >= 1)
        {
            Renderer::Projection = glm::perspective(glm::radians(m_CurrentCamera->Fov),
                                                      Renderer::RenderSize.x / Renderer::RenderSize.y, 0.1f,
                                                      100.0f);
            Renderer::View = m_CurrentCamera->GetViewMatrix();
        }
        glfwPollEvents();

        Renderer::Bind(); // Bind the framebuffer
        for (int i = 0; i < m_GameLoops.size(); i++) { m_GameLoops[i](); }
        for (int i = 0; i < m_Objects->size(); i++) { m_Objects->at(i)->_Update(); }
        Renderer::Unbind(); // Unbind and set the framebuffer

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        UIManager::Update(); // Update the ImGui Context and show the framebuffer
        
        glfwSwapBuffers(Renderer::GetWindow());

        Renderer::RenderTimeInMs = timer.ElapsedMillis();
    }
}

void GameEngine::Engine::UpdateShaders()
{
    for (int i = 0; i < m_Shaders.size(); i++)
    {
        m_Shaders.at(i)->Use();
        m_Shaders.at(i)->SetMat4("projection", Renderer::Projection);
        m_Shaders.at(i)->SetMat4("view", Renderer::View);
    }
}
