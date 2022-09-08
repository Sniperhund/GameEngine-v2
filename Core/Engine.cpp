#include <glad/glad.h>
#include "Engine.h"
#include "Util/Debug.h"
#include "EditorCamera.h"
#include "Util/Timer.h"

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
	AddLayer(m_Hierarchy);
	AddLayer(m_PerformanceView);
	AddLayer(m_Properties);
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

	//ImGui::ShowDemoWindow();

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
	Object* object = new Object();
	object->SetInfo("Shader", "cube.fbx");
	object->SetName("Object");
	object->SetRotation(glm::vec3(30));
	AddObject(object);
	Object* object2 = new Object();
	object2->SetName("Object 2");
	object2->SetInfo("Shader", "cube.fbx");
	object2->SetRotation(glm::vec3(60));
	AddObject(object2);
	Object* object3 = new Object();
	object3->SetName("Object 3");
	object3->SetInfo("Shader", "cube.fbx");
	object3->SetRotation(glm::vec3(160));
	AddObject(object3);

	m_Renderer->CreateFramebuffer();
	
	Camera camera;
	SetCurrentCamera(&camera);

	while (!glfwWindowShouldClose(m_Renderer->GetWindow())) {
		Timer timer;
		m_Renderer->UpdateDeltaTime();
		camera.HandleInput(m_Renderer);
		glfwPollEvents();

		m_Renderer->Bind();

		for (int i = 0; i < m_GameLoops.size(); i++) { m_GameLoops[i](); }
		for (int i = 0; i < m_Objects.size(); i++) { m_Objects[i]->_Update(m_CurrentCamera); }

		m_Renderer->Unbind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		UpdateImGui();

		glfwSwapBuffers(m_Renderer->GetWindow());

		m_Renderer->RenderTimeInMs = timer.ElapsedMillis();
	}
}