#include "SceneView.h"

#include "../../Graphics/Renderer.h"
#include "../../Util/Debug.h"

// UI crashes but I cant find out why, maybe ask the discord.

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