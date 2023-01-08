#include "Properties.h"

#include "../../Graphics/Renderer.h"

#include <imgui_stdlib.h>
#include "../../Engine.h"
#include "../../Util.h"

void GameEngine::Properties::Start()
{
    title = "Properties";
    active = true;
}

void GameEngine::Properties::Update()
{
    for (int i = 0; i < Engine::m_Objects->size(); i++)
    {
        if (Renderer::Selected != i) continue;

        ImGui::Text("General Settings");
        std::string name = Engine::m_Objects->at(i)->GetName();
        if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
            Engine::m_Objects->at(i)->SetName(name);

        ImGui::Text("Transform");
        glm::vec3 _pos = Engine::m_Objects->at(i)->GetPosition();
        float pos[3] = {_pos.x, _pos.y, _pos.z};
        ImGui::InputFloat3("Position", pos);
        Engine::m_Objects->at(i)->SetPosition(glm::vec3(pos[0], pos[1], pos[2]));

        glm::vec3 _scale = Engine::m_Objects->at(i)->GetScale();
        float scale[3] = {_scale.x, _scale.y, _scale.z};
        ImGui::InputFloat3("Scale", scale);
        Engine::m_Objects->at(i)->SetScale(glm::vec3(scale[0], scale[1], scale[2]));

        glm::vec3 _rotation = Engine::m_Objects->at(i)->GetRotation();
        float rotation[3] = {_rotation.x, _rotation.y, _rotation.z};
        ImGui::InputFloat3("Rotation", rotation);
        Engine::m_Objects->at(i)->SetRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));

        ImGui::Text("Rendering");
        glm::vec4 _col = Engine::m_Objects->at(i)->GetColor();
        float col[4] = {_col.x, _col.y, _col.z, _col.w};
        float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.50f;
        ImGui::SetNextItemWidth(w);
        ImGui::ColorPicker4("Color", col, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
        Engine::m_Objects->at(i)->SetColor(glm::vec4(col[0], col[1], col[2], col[3]));

        static int item_current = 0;
        for (int j = 0; j < IM_ARRAYSIZE(shaders); j++)
        {
            if (Engine::m_Objects->at(i)->GetShader() == shaders[j]) item_current = j;
        }
        ImGui::Combo("combo", &item_current, shaders, IM_ARRAYSIZE(shaders));
        if (Engine::m_Objects->at(i)->GetShader() != shaders[item_current])
            Engine::m_Objects->at(i)->SetShader(shaders[item_current]);
    }
}