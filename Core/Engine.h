#pragma once
#include "Renderer.h"
#include <vector>
#include <functional>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

namespace GameEngine {
	class Engine
	{
	private:
		Renderer* m_Renderer;

		std::vector<std::function<void()>> m_GameLoops;
	public:
		Engine(Renderer* renderer) : m_Renderer(renderer) {
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.WantCaptureMouse = true;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL(m_Renderer->GetWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 330");
		}

		void UpdateImGui();

		void AddGameLoopCallback(std::function<void()> callback) { m_GameLoops.push_back(std::move(callback)); }
		void StartGameLoop();

	private:
		bool m_showImGuiDemo;
	};
}
