#include "Engine.h"

void GameEngine::Engine::UpdateImGui()
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (m_showImGuiDemo) ImGui::ShowDemoWindow(&m_showImGuiDemo);

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Windows")) {
		ImGui::MenuItem("Show Engine");

		ImGui::EndMenu();
	}
#ifdef _DEBUG
	if (ImGui::BeginMenu("Engine Development"))
	{
		ImGui::MenuItem("Show ImGui Examples/Demos", nullptr, &m_showImGuiDemo);

		ImGui::EndMenu();
	}
#endif
	ImGui::EndMainMenuBar();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}

void GameEngine::Engine::StartGameLoop()
{
	while (!glfwWindowShouldClose(m_Renderer->GetWindow())) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();

		UpdateImGui();

		for (int i = 0; i < m_GameLoops.size(); i++) {
			m_GameLoops[i]();
		}

		glfwSwapBuffers(m_Renderer->GetWindow());
	}
}