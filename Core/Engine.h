#pragma once
#include "Graphics/Renderer.h"
#include <vector>
#include <functional>

#include <imgui.h>
#include <memory>

#include "EditorCamera.h"
#include "Graphics/Shader.h"
#include "UI/UILayer.h"
#include "UI/Layers/Hierarchy.h"
#include "UI/Layers/PerformanceView.h"
#include "UI/Layers/Properties.h"
#include "UI/Layers/SceneView.h"

namespace GameEngine
{
    class Engine
    {
    private:
        static std::vector<std::function<void()>> m_GameLoops;
        static std::vector<std::shared_ptr<Shader>> m_Shaders;
    public:
        static Camera* m_CurrentCamera;
        static std::shared_ptr<std::vector<std::shared_ptr<Object>>> m_Objects;
        static void Init();

        static void SetCurrentCamera(Camera* camera) { m_CurrentCamera = camera; }
        
        static std::shared_ptr<Object> CreateObject(std::string name, std::string shaderPath, std::string modelPath);
        static void AddObject(std::shared_ptr<Object> object);

        static void AddGameLoopCallback(std::function<void()> callback) { m_GameLoops.push_back(std::move(callback)); }
        static void StartGameLoop();
    private:
        static void UpdateShaders();
    };
}
