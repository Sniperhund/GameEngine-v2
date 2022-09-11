﻿#include "Entity.h"

#include <utility>

void GameEngine::Entity::_Start(Renderer* renderer)
{
    m_Renderer = renderer;
    Start();
}

void GameEngine::Entity::_Update(Camera* currentCamera)
{
    m_CurrentCamera = currentCamera;
    Update();
}

void GameEngine::Object::SetPosition(glm::vec3 position)
{
    m_Position = position;
}

void GameEngine::Object::SetRotation(glm::vec3 rotation)
{
    m_Rotation = rotation;
}

void GameEngine::Object::SetScale(glm::vec3 scale)
{
    m_Scale = scale;
}

void GameEngine::Object::SetColor(glm::vec4 color)
{
    m_Color = color;
}

void GameEngine::Object::SetName(std::string name)
{
    m_Name = name;
}

void GameEngine::Object::SetShader(std::string shader)
{
    m_ShaderPath = shader;
    m_Shader.Init(std::format("{}.vert", m_ShaderPath).c_str(),
                  std::format("{}.frag", m_ShaderPath).c_str());
}

glm::vec3 GameEngine::Object::GetPosition()
{
    return m_Position;
}

glm::vec3 GameEngine::Object::GetRotation()
{
    return m_Rotation;
}

glm::vec3 GameEngine::Object::GetScale()
{
    return m_Scale;
}

glm::vec4 GameEngine::Object::GetColor()
{
    return m_Color;
}

std::string GameEngine::Object::GetName()
{
    return m_Name;
}

UUIDv4::UUID GameEngine::Object::GetUUID()
{
    return m_uuid;
}

std::string GameEngine::Object::GetShader()
{
    return m_ShaderPath;
}

std::string GameEngine::Object::GetModel()
{
    return m_ModelPath;
}

void GameEngine::Object::_Start(Renderer* renderer, std::shared_ptr<std::vector<std::shared_ptr<Object>>> objects)
{
    if (m_IsInit) return;
    
    m_Objects = std::move(objects);
    m_IsInit = true;
    m_Renderer = renderer;
    Start();
}

void GameEngine::Object::_Update(Camera* currentCamera)
{
    m_CurrentCamera = currentCamera;
    Update();
}

void GameEngine::Object::SetInfo(std::string shaderPath, std::string modelPath)
{
    m_ShaderPath = shaderPath;
    m_ModelPath = modelPath;
}

void GameEngine::Object::Start()
{
    if (m_ShaderPath.empty() && m_ModelPath.empty())
    {
        DebugError("Something went wrong. Somehow someone forgot to set the model path or shader path");
    }

    m_Shader.Init(std::format("{}.vert", m_ShaderPath).c_str(), std::format("{}.frag", m_ShaderPath).c_str());
    m_Model.Init(m_ModelPath);
    
}

void GameEngine::Object::Update()
{
    if (m_Renderer->RenderSize.x >= 1 && m_Renderer->RenderSize.y >= 1)
    {
        m_Shader.Use();

        auto model = glm::mat4(1.0f);
        model = translate(model, m_Position);
        model = rotate(model, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
        model = rotate(model, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
        model = rotate(model, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
        model = scale(model, m_Scale);
        m_Shader.SetMat4("model", model);

        m_Shader.SetVec4("custom_color", m_Color);

        m_Model.Draw(m_Shader);
    }
}