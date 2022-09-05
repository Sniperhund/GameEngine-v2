#include <glad/glad.h>
#include "Engine.h"
#include "Debug.h"
#include "EditorCamera.h"
#include "Model.h"
#include "Timer.h"

GameEngine::Engine::Engine(Renderer* renderer) : m_Renderer(renderer) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.WantCaptureMouse = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(m_Renderer->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

	AddLayer(m_SceneView);
	AddLayer(m_PerformanceView);
}

void GameEngine::Engine::AddLayer(UILayer* layer) {
	m_Layers.push_back(std::move(layer));
}

void GameEngine::Engine::UpdateImGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
	ImGui::PopStyleVar(3);

	ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::BeginMenuBar();
	if (ImGui::BeginMenu("Windows")) {
		for (auto& layer : m_Layers) {
			if (ImGui::MenuItem(layer->title.c_str(), NULL, layer->active)) layer->active = !layer->active;
		}
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
	ImGui::End();

	for (auto& layer : m_Layers) {
		layer->_Update();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void GameEngine::Engine::StartGameLoop()
{
	for (auto layer : m_Layers) {
		layer->_Start(m_Renderer);
	}

	m_Renderer->CreateFramebuffer();

	Shader shader;
	Camera camera;
	Model cube;

	shader.Init("Shader.vert", "Shader.frag");
	cube.Init("cube.fbx");

	while (!glfwWindowShouldClose(m_Renderer->GetWindow())) {
		Timer timer;
		m_Renderer->UpdateDeltaTime();
		camera.HandleInput(m_Renderer);
		glfwPollEvents();

		m_Renderer->Bind();

		for (int i = 0; i < m_GameLoops.size(); i++) {
			m_GameLoops[i]();
		}

		if (m_Renderer->RenderSize.x >= 1 && m_Renderer->RenderSize.y >= 1) {
			shader.Use();

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0, 0, 0));
			shader.SetMat4("model", model);

			glm::mat4 projection = glm::perspective(glm::radians(camera.Fov),
				(float)m_Renderer->RenderSize.x / (float)m_Renderer->RenderSize.y, 0.1f, 100.0f);
			shader.SetMat4("projection", projection);
			glm::mat4 view = camera.GetViewMatrix();
			shader.SetMat4("view", view);

			cube.Draw(shader);
		}

		m_Renderer->Unbind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		UpdateImGui();

		glfwSwapBuffers(m_Renderer->GetWindow());

		m_Renderer->RenderTimeInMs = timer.ElapsedMillis();
	}
}