#pragma once
#include <format>

#include "Renderer.h"

#include <format>

#include <imgui.h>
#include <imgui_stdlib.h>

namespace GameEngine {
	class UILayer {
	protected:
		bool removePadding = false;
		ImGuiWindowFlags windowFlags;
		Renderer* m_Renderer;
	public:
		void _Update() {
			if (!active) return;

			if (removePadding) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			if (!ImGui::Begin(title.c_str(), &active, windowFlags)) {
				ImGui::End();
				return;
			}

			Update();

			ImGui::End();
			if (removePadding) ImGui::PopStyleVar();
		}

		void _Start(Renderer* renderer) {
			m_Renderer = renderer;
			Start();
		}

		bool active = false;
		std::string title = "Default name";

		virtual ~UILayer() = default;

		virtual void Start() {}
		virtual void Update() {}
	};

	class SceneView : public UILayer {
		void Start() override {
			title = "SceneView";
			removePadding = true;
		}

		void Update() override {
			if (!active) return;

			ImVec2 view = ImGui::GetContentRegionAvail();
			m_Renderer->Resize(view.x, view.y);
			ImGui::Image(reinterpret_cast<void*>(m_Renderer->GetTexture()),
				ImVec2{ m_Renderer->RenderSize.x, m_Renderer->RenderSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}
	};

	class PerformanceView : public UILayer {
		int i = 0;
		float lastRenderTime = 0;

		void Start() override {
			title = "PerformanceView";
		}

		void Update() override {
			if (!active) return;

			if (i == 5)
			{
				lastRenderTime = m_Renderer->RenderTimeInMs;
				ImGui::Text(std::format("It took {} ms to render", round(lastRenderTime * 100) / 100).c_str());
				i = 0;
				return;
			}
			ImGui::Text(std::format("It took {} ms to render", round(lastRenderTime * 100) / 100).c_str());
			i++;
		}
	};
}