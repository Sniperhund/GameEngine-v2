#pragma once
#include "Renderer.h"
#include <vector>
#include <functional>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>
#include <memory>
#include "UILayer.h"

namespace GameEngine {
	class Engine
	{
	private:
		Renderer* m_Renderer;

		std::vector<std::function<void()>> m_GameLoops;
		std::vector<UILayer*> m_Layers;
		
		UILayer* m_SceneView = new SceneView();
		UILayer* m_PerformanceView = new PerformanceView();
	public:
		Engine(Renderer* renderer);

		void UpdateImGui();

		void AddLayer(UILayer* layer);
		void AddGameLoopCallback(std::function<void()> callback) { m_GameLoops.push_back(std::move(callback)); }
		void StartGameLoop();
	private:
		bool m_showImGuiDemo;
	};
}
