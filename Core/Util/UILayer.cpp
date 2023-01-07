#include "UILayer.h"

#include <imgui_stdlib.h>
#include <nfd.h>

#include <format>

#include "../Util.h"

void GameEngine::UILayer::_Update()
{
    if (!active) return;

    if (removePadding) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (!ImGui::Begin(title.c_str(), &active, windowFlags))
    {
        ImGui::End();
        return;
    }

    Update();

    ImGui::End();
    if (removePadding) ImGui::PopStyleVar();
}

void GameEngine::UILayer::_Start()
{
    Start();
}

void GameEngine::SceneView::Start()
{
    title = "SceneView";
    removePadding = true;
    active = true;
}

void GameEngine::SceneView::Update()
{
    ImVec2 view = ImGui::GetContentRegionAvail();
    Renderer::Resize(view.x, view.y);
    ImGui::Image(reinterpret_cast<void*>(Renderer::GetTexture()),
                 ImVec2{Renderer::RenderSize.x, Renderer::RenderSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
}

GameEngine::Hierarchy::Hierarchy(std::shared_ptr<std::vector<std::shared_ptr<Object>>> objects)
{
    m_PointerToObjectsArray = std::move(objects);
}

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
                _object->_Start(m_PointerToObjectsArray);
                m_PointerToObjectsArray->emplace_back(std::move(_object));
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Cube"))
            {
                _object->SetName("Cube");
                _object->SetInfo(defaultShader, cubeModel);
                _object->_Start(m_PointerToObjectsArray);
                m_PointerToObjectsArray->emplace_back(std::move(_object));
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Sphere"))
            {
                _object->SetName("Sphere");
                _object->SetInfo(defaultShader, sphereModel);
                _object->_Start(m_PointerToObjectsArray);
                m_PointerToObjectsArray->emplace_back(std::move(_object));
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Cylinder"))
            {
                _object->SetName("Cylinder");
                _object->SetInfo(defaultShader, cylinderModel);
                _object->_Start(m_PointerToObjectsArray);
                m_PointerToObjectsArray->emplace_back(std::move(_object));
                ImGui::CloseCurrentPopup();
            }

            ImGui::TreePop();
        }
        ImGui::EndPopup();
    }

    for (int i = 0; i < m_PointerToObjectsArray->size(); i++)
    {
        if (ImGui::Selectable(std::format("{}##{}", m_PointerToObjectsArray->at(i)->GetName(),
                                          m_PointerToObjectsArray->at(i)->GetUUID().str()).c_str(),
                              Renderer::Selected == i))
        {
            Renderer::Selected = i;
        }
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::Button("Delete Object"))
            {
                // TODO: Make undo list
                m_PointerToObjectsArray->erase(m_PointerToObjectsArray->begin() + i);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Create Object"))
            {
                m_openNewObject = true;
            }
            if (ImGui::Button("Duplicate Object"))
            {
                _object->SetColor(m_PointerToObjectsArray->at(i)->GetColor());
                _object->SetInfo(m_PointerToObjectsArray->at(i)->GetShader(),
                                 m_PointerToObjectsArray->at(i)->GetModel());
                _object->SetName(std::format("{} (duplicate)", m_PointerToObjectsArray->at(i)->GetName()));
                _object->SetPosition(m_PointerToObjectsArray->at(i)->GetPosition());
                _object->SetRotation(m_PointerToObjectsArray->at(i)->GetRotation());
                _object->SetScale(m_PointerToObjectsArray->at(i)->GetScale());
                _object->_Start(m_PointerToObjectsArray);
                m_PointerToObjectsArray->emplace_back(std::move(_object));
                ImGui::CloseCurrentPopup();
            }
            ImGui::Separator();
            if (ImGui::TreeNodeEx("3D Objects", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::Button("Plane"))
                {
                    _object->SetName("Plane");
                    _object->SetInfo(defaultShader, planeModel);
                    _object->_Start(m_PointerToObjectsArray);
                    m_PointerToObjectsArray->emplace_back(std::move(_object));
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("Cube"))
                {
                    _object->SetName("Cube");
                    _object->SetInfo(defaultShader, cubeModel);
                    _object->_Start(m_PointerToObjectsArray);
                    m_PointerToObjectsArray->emplace_back(std::move(_object));
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("Sphere"))
                {
                    _object->SetName("Sphere");
                    _object->SetInfo(defaultShader, sphereModel);
                    _object->_Start(m_PointerToObjectsArray);
                    m_PointerToObjectsArray->emplace_back(std::move(_object));
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("Cylinder"))
                {
                    _object->SetName("Cylinder");
                    _object->SetInfo(defaultShader, cylinderModel);
                    _object->_Start(m_PointerToObjectsArray);
                    m_PointerToObjectsArray->emplace_back(std::move(_object));
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
        _object->_Start(m_PointerToObjectsArray);
        m_PointerToObjectsArray->emplace_back(std::move(_object));

        m_name = "";
        m_shaderPath = "";
        m_modelPath = "";

        *p_open = false;
    }

    ImGui::End();
}

GameEngine::Properties::Properties(std::shared_ptr<std::vector<std::shared_ptr<Object>>> objects)
{
    m_PointerToObjectsArray = std::move(objects);
}

void GameEngine::Properties::Start()
{
    title = "Properties";
    active = true;
}

void GameEngine::Properties::Update()
{
    for (int i = 0; i < m_PointerToObjectsArray->size(); i++)
    {
        if (Renderer::Selected != i) continue;

        ImGui::Text("General Settings");
        std::string name = m_PointerToObjectsArray->at(i)->GetName();
        if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
            m_PointerToObjectsArray->at(i)->SetName(name);

        ImGui::Text("Transform");
        glm::vec3 _pos = m_PointerToObjectsArray->at(i)->GetPosition();
        float pos[3] = {_pos.x, _pos.y, _pos.z};
        ImGui::InputFloat3("Position", pos);
        m_PointerToObjectsArray->at(i)->SetPosition(glm::vec3(pos[0], pos[1], pos[2]));

        glm::vec3 _scale = m_PointerToObjectsArray->at(i)->GetScale();
        float scale[3] = {_scale.x, _scale.y, _scale.z};
        ImGui::InputFloat3("Scale", scale);
        m_PointerToObjectsArray->at(i)->SetScale(glm::vec3(scale[0], scale[1], scale[2]));

        glm::vec3 _rotation = m_PointerToObjectsArray->at(i)->GetRotation();
        float rotation[3] = {_rotation.x, _rotation.y, _rotation.z};
        ImGui::InputFloat3("Rotation", rotation);
        m_PointerToObjectsArray->at(i)->SetRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));

        ImGui::Text("Rendering");
        glm::vec4 _col = m_PointerToObjectsArray->at(i)->GetColor();
        float col[4] = {_col.x, _col.y, _col.z, _col.w};
        float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.50f;
        ImGui::SetNextItemWidth(w);
        ImGui::ColorPicker4("Color", col, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
        m_PointerToObjectsArray->at(i)->SetColor(glm::vec4(col[0], col[1], col[2], col[3]));

        static int item_current = 0;
        for (int j = 0; j < IM_ARRAYSIZE(shaders); j++)
        {
            if (m_PointerToObjectsArray->at(i)->GetShader() == shaders[j]) item_current = j;
        }
        ImGui::Combo("combo", &item_current, shaders, IM_ARRAYSIZE(shaders));
        if (m_PointerToObjectsArray->at(i)->GetShader() != shaders[item_current])
            m_PointerToObjectsArray->at(i)->SetShader(shaders[item_current]);
    }
}

void GameEngine::PerformanceView::Start()
{
    title = "PerformanceView";
}

void GameEngine::PerformanceView::Update()
{
    if (!active) return;

    lastRenderTime = Renderer::RenderTimeInMs;
    ImGui::Text(std::format("Frame Time: {}ms", round(lastRenderTime * 100) / 100).c_str());

    if (ImGui::TreeNode("CPU Timers"))
    {
        // TODO: Add CPU timing
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("GPU Timers"))
    {
        // TODO: Add GPU timing
        ImGui::TreePop();
    }
}
