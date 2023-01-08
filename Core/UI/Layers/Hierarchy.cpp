#include "Hierarchy.h"

#include <nfd.h>

#include <imgui_stdlib.h>
#include <memory>
#include <nfd.h>

#include "../../Engine.h"
#include "../../Util.h"

void GameEngine::Hierarchy::Start()
{
    title = "Hierarchy";
    active = true;
}

void GameEngine::Hierarchy::Update()
{
    auto _object = std::make_shared<Object>();
    if (m_openNewObject) ShowNewObject(&m_openNewObject);

    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Button("Create Object"))
        {
            m_openNewObject = true;
        }
        ImGui::Separator();
        // All objects needs to reworked to work with a scene object instead of this.
        if (ImGui::TreeNodeEx("3D Objects", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::Button("Plane"))
            {
                _object->SetName("Plane");
                _object->SetInfo(defaultShader, planeModel);
                _object->_Start();
                Engine::m_Objects->emplace_back(std::move(_object));
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Cube"))
            {
                _object->SetName("Cube");
                _object->SetInfo(defaultShader, cubeModel);
                _object->_Start();
                Engine::m_Objects->emplace_back(std::move(_object));
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Sphere"))
            {
                _object->SetName("Sphere");
                _object->SetInfo(defaultShader, sphereModel);
                _object->_Start();
                Engine::m_Objects->emplace_back(std::move(_object));
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Cylinder"))
            {
                _object->SetName("Cylinder");
                _object->SetInfo(defaultShader, cylinderModel);
                _object->_Start();
                Engine::m_Objects->emplace_back(std::move(_object));
                ImGui::CloseCurrentPopup();
            }

            ImGui::TreePop();
        }
        ImGui::EndPopup();
    }

    for (int i = 0; i < Engine::m_Objects->size(); i++)
    {
        if (ImGui::Selectable(std::format("{}##{}", Engine::m_Objects->at(i)->GetName(),
                                          Engine::m_Objects->at(i)->GetUUID().str()).c_str(),
                              Renderer::Selected == i))
        {
            Renderer::Selected = i;
        }
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::Button("Delete Object"))
            {
                // TODO: Make undo list
                Engine::m_Objects->erase(Engine::m_Objects->begin() + i);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Create Object"))
            {
                m_openNewObject = true;
            }
            if (ImGui::Button("Duplicate Object"))
            {
                _object->SetColor(Engine::m_Objects->at(i)->GetColor());
                _object->SetInfo(Engine::m_Objects->at(i)->GetShader(),
                                 Engine::m_Objects->at(i)->GetModel());
                _object->SetName(std::format("{} (duplicate)", Engine::m_Objects->at(i)->GetName()));
                _object->SetPosition(Engine::m_Objects->at(i)->GetPosition());
                _object->SetRotation(Engine::m_Objects->at(i)->GetRotation());
                _object->SetScale(Engine::m_Objects->at(i)->GetScale());
                _object->_Start();
                Engine::m_Objects->emplace_back(std::move(_object));
                ImGui::CloseCurrentPopup();
            }
            ImGui::Separator();
            if (ImGui::TreeNodeEx("3D Objects", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::Button("Plane"))
                {
                    _object->SetName("Plane");
                    _object->SetInfo(defaultShader, planeModel);
                    _object->_Start();
                    Engine::m_Objects->emplace_back(std::move(_object));
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("Cube"))
                {
                    _object->SetName("Cube");
                    _object->SetInfo(defaultShader, cubeModel);
                    _object->_Start();
                    Engine::m_Objects->emplace_back(std::move(_object));
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("Sphere"))
                {
                    _object->SetName("Sphere");
                    _object->SetInfo(defaultShader, sphereModel);
                    _object->_Start();
                    Engine::m_Objects->emplace_back(std::move(_object));
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("Cylinder"))
                {
                    _object->SetName("Cylinder");
                    _object->SetInfo(defaultShader, cylinderModel);
                    _object->_Start();
                    Engine::m_Objects->emplace_back(std::move(_object));
                    ImGui::CloseCurrentPopup();
                }

                ImGui::TreePop();
            }
            ImGui::EndPopup();
        }
    }
}

void GameEngine::Hierarchy::ShowNewObject(bool* p_open)
{
    if (!ImGui::Begin("New Object", p_open))
    {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Find model"))
    {
        nfdchar_t* outPath = nullptr;
        nfdresult_t result = NFD_OpenDialog("fbx,obj", nullptr, &outPath);

        if (result == NFD_OKAY)
        {
            m_modelPath = outPath;
        }
    }
    ImGui::SameLine();
    ImGui::Text(m_modelPath.c_str());
    static int item_current = 0;
    ImGui::Combo("combo", &item_current, shaders, IM_ARRAYSIZE(shaders));
    m_shaderPath = shaders[item_current];
    ImGui::InputText("Name", &m_name);

    ImGui::Spacing();
    if (ImGui::Button("Submit"))
    {
        auto _object = std::make_shared<Object>();
        if (m_name.empty()) m_name = "Default Name";
        _object->SetName(m_name);
        _object->SetInfo(m_shaderPath, m_modelPath);
        _object->_Start();
        Engine::m_Objects->emplace_back(std::move(_object));

        m_name = "";
        m_shaderPath = "";
        m_modelPath = "";

        *p_open = false;
    }

    ImGui::End();
}