#pragma once

#include <glm/glm.hpp>
#include <format>

#include "../Vendors/uuid_v4/uuid_v4.h"

#include "../EditorCamera.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Model.h"
#include "../Graphics/Renderer.h"

namespace GameEngine
{
    class Entity
    {
    protected:
        glm::vec3 m_Position = glm::vec3(0);
        glm::vec3 m_Scale = glm::vec3(0);
        glm::vec3 m_Rotation = glm::vec3(0);
        
        Camera* m_CurrentCamera;
    public:
        virtual void _Start();

        virtual void _Update(Camera* currentCamera);

        virtual void Start() {}

        virtual void Update() {}
    };

    class Object
    {
    private:
        bool m_IsInit = false;

        Shader m_Shader;
        Model m_Model;

        std::string m_ShaderPath = "";
        std::string m_ModelPath = "";

        glm::vec3 m_Position = glm::vec3(0);
        glm::vec3 m_Scale = glm::vec3(1);
        glm::vec3 m_Rotation = glm::vec3(0);
        glm::vec4 m_Color = glm::vec4(0);
        
        Camera* m_CurrentCamera = nullptr;
        std::shared_ptr<std::vector<std::shared_ptr<Object>>> m_Objects;
        std::string m_Name = "Default Name";
        UUIDv4::UUID m_uuid = UUIDv4::UUIDGenerator<std::mt19937>().getUUID();
    public:
        void SetPosition(glm::vec3 position);
        void SetRotation(glm::vec3 rotation);
        void SetScale(glm::vec3 scale);
        void SetColor(glm::vec4 color);
        void SetName(std::string name);
        void SetShader(std::string shader);

        glm::vec3 GetPosition();
        glm::vec3 GetRotation();
        glm::vec3 GetScale();
        glm::vec4 GetColor();
        std::string GetName();
        UUIDv4::UUID GetUUID();
        std::string GetShader();
        std::string GetModel();

        void _Start(std::shared_ptr<std::vector<std::shared_ptr<Object>>> objects);
        void _Update(Camera* currentCamera);

        virtual void SetInfo(std::string shaderPath, std::string modelPath);

        virtual void Start();
        virtual void Update();
    };
}
