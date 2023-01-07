#pragma once

#include "../Graphics/Renderer.h"
#include "Entity.h"

#include <imgui.h>

namespace GameEngine
{
    class UILayer
    {
    protected:
        bool removePadding = false;
        ImGuiWindowFlags windowFlags;
    public:
        void _Update();

        void _Start();

        bool active = false;
        std::string title = "Default name";

        virtual ~UILayer() = default;

        virtual void Start()
        {
        }

        virtual void Update()
        {
        }
    };

    class SceneView : public UILayer
    {
        void Start() override;

        void Update() override;
    };

    class Hierarchy : public UILayer
    {
        std::shared_ptr<std::vector<std::shared_ptr<Object>>> m_PointerToObjectsArray;
        bool m_openNewObject = false;

        std::string m_modelPath;
        std::string m_shaderPath;
        std::string m_name;
    public:
        Hierarchy(std::shared_ptr<std::vector<std::shared_ptr<Object>>> objects);

        void Start() override;

        std::string defaultShader = "Unlit";

        std::string planeModel = "Assets/Primitive/Plane.fbx";
        std::string cubeModel = "Assets/Primitive/Cube.fbx";
        std::string sphereModel = "Assets/Primitive/Sphere.fbx";
        std::string cylinderModel = "Assets/Primitive/Cylinder.fbx";

        void Update() override;
    private:
        void ShowNewObject(bool* p_open);
    };

    class Properties : public UILayer
    {
    private:
        std::shared_ptr<std::vector<std::shared_ptr<Object>>> m_PointerToObjectsArray;
    public:
        Properties(std::shared_ptr<std::vector<std::shared_ptr<Object>>> objects);

        void Start() override;

        void Update() override;
    };

    class PerformanceView : public UILayer
    {
        float lastRenderTime = 0;

        void Start() override;

        void Update() override;
    };
}
