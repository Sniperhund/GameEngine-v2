#pragma once
#include "Graphics/Renderer.h"
#include <vector>
#include <functional>

#include <imgui.h>
#include <memory>

#include "EditorCamera.h"
#include "Util/UILayer.h"

namespace GameEngine
{
    class Engine
    {
    private:
        std::vector<std::function<void()>> m_GameLoops;
        std::vector<UILayer*> m_Layers;
        std::shared_ptr<std::vector<std::shared_ptr<Object>>> m_Objects = std::make_shared<std::vector<std::shared_ptr<
            Object>>>();
        std::vector<std::shared_ptr<Shader>> m_Shaders;

        UILayer* m_SceneView = new SceneView();
        UILayer* m_PerformanceView = new PerformanceView();
        UILayer* m_Hierarchy = new Hierarchy(m_Objects);
        UILayer* m_Properties = new Properties(m_Objects);

        Camera* m_CurrentCamera;
    public:
        Engine();

        void AddShader(std::shared_ptr<Shader> shader);

        void SetCurrentCamera(Camera* camera) { m_CurrentCamera = camera; }
        
        std::shared_ptr<Object> CreateObject(std::string name, std::string shaderPath, std::string modelPath);
        void AddObject(std::shared_ptr<Object> object);
        void AddLayer(UILayer* layer);

        void AddGameLoopCallback(std::function<void()> callback) { m_GameLoops.push_back(std::move(callback)); }
        void StartGameLoop();
    private:
        void UpdateShaders();
        void SetupImGuiStyle();
        void UpdateImGui();
    };
}
