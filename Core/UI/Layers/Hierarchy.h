#pragma once

#include <memory>
#include <vector>

#include "../UILayer.h"
#include "../../Util/Entity.h"

namespace GameEngine
{
    class Hierarchy : public UILayer
    {
        bool m_openNewObject = false;

        std::string m_modelPath;
        std::string m_shaderPath;
        std::string m_name;
    public:
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
}