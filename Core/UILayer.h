#pragma once

#include <xstring>

#include <imgui.h>
#include <imgui_stdlib.h>

namespace GameEngine {
	class UILayer {
	public:
		void _Update() {
			if (!active) return;

			if (!ImGui::Begin(title.c_str(), &active, windowFlags)) {
				ImGui::End();
				return;
			}

			Update();

			ImGui::End();
		}

		bool active = false;
		std::string title = "Default name";
		ImGuiWindowFlags windowFlags;

		virtual ~UILayer() = default;

		virtual void Start() {}
		virtual void Update() {}
	};
}