#pragma once
#include "Renderer.h"

#include <xstring>

#include <imgui.h>
#include <imgui_stdlib.h>

namespace GameEngine {
	class UILayer {
	protected:
		bool removePadding = false;
		ImGuiWindowFlags windowFlags;
		Renderer* _Renderer;
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
			_Renderer = renderer;
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
			_Renderer->Resize(view.x, view.y);
			ImGui::Image(reinterpret_cast<void*>(_Renderer->GetTexture()),
				ImVec2{ _Renderer->RenderSize.x, _Renderer->RenderSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}
	};
}