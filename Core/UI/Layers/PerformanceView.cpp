#include "PerformanceView.h"

#include <format>
#include "../../Graphics/Renderer.h"

void GameEngine::PerformanceView::Start()
{
    title = "PerformanceView";
}

void GameEngine::PerformanceView::Update()
{
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