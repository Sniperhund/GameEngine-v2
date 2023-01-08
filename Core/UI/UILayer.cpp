#include "UILayer.h"

#include <format>

void GameEngine::UILayer::_Update()
{
    if (!active) return;

    if (removePadding) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (!ImGui::Begin(title.c_str(), &active))
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
