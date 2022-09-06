#pragma once
#include "Renderer.h"
#include <vector>
#include <functional>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>
#include <memory>

#include "EditorCamera.h"
#include "Entity.h"
#include "UILayer.h"

namespace GameEngine {
	class Engine
	{
	private:
		Renderer* m_Renderer;

		std::vector<std::function<void()>> m_GameLoops;
		std::vector<UILayer*> m_Layers;
		std::vector<Object*> m_Objects;
		
		UILayer* m_SceneView = new SceneView();
		UILayer* m_PerformanceView = new PerformanceView();

		Camera* m_CurrentCamera;
	public:
		Engine(Renderer* renderer);

		void RefreshObjects()
		{
			for (auto layer : m_Layers) { layer->_Start(m_Renderer); }
			for (auto object : m_Objects) { object->_Start(m_Renderer); }
		}

		void UpdateImGui();

		void SetCurrentCamera(Camera* camera) { m_CurrentCamera = camera; }

		void AddObject(Object* object) { m_Objects.emplace_back(std::move(object)); RefreshObjects(); }
		void AddLayer(UILayer* layer) { m_Layers.push_back(std::move(layer)); RefreshObjects(); }
		void AddGameLoopCallback(std::function<void()> callback) { m_GameLoops.push_back(std::move(callback)); }
		void StartGameLoop();
	private:
		bool m_showImGuiDemo;
	};
}
