#pragma once
#include <format>

#include "../Graphics/Renderer.h"

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
			ImVec2 view = ImGui::GetContentRegionAvail();
			m_Renderer->Resize(view.x, view.y);
			ImGui::Image(reinterpret_cast<void*>(m_Renderer->GetTexture()),
				ImVec2{ m_Renderer->RenderSize.x, m_Renderer->RenderSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}
	};

	class Hierarchy : public UILayer {
		const int m_MaxNumOfObjects;
		std::shared_ptr<std::vector<std::shared_ptr<Object>>> m_PointerToObjectsArray;
	public:
		Hierarchy(std::shared_ptr<std::vector<std::shared_ptr<Object>>> objects, int maxNumOfObjects) : m_MaxNumOfObjects(maxNumOfObjects)
		{
			m_PointerToObjectsArray = std::move(objects);
		}

		void Start() override {
			title = "Hierarchy";
		}

		void Update() override {

			
			for (int i = 0; i < m_PointerToObjectsArray->size(); i++) {
				if (ImGui::Selectable(std::format("{}##{}", m_PointerToObjectsArray->at(i)->GetName(),
					m_PointerToObjectsArray->at(i)->GetUUID().str()).c_str(), m_Renderer->Selected == i))
					m_Renderer->Selected = i;
			}
		}
	};

	class Properties : public UILayer
	{
	private:
		std::shared_ptr<std::vector<std::shared_ptr<Object>>> m_PointerToObjectsArray;
	public:
		Properties(std::shared_ptr<std::vector<std::shared_ptr<Object>>> objects)
		{
			m_PointerToObjectsArray = std::move(objects);
		}

		void Start() override {
			title = "Properties";
		}

		void Update() override
		{
			for (int i = 0; i < m_PointerToObjectsArray->size(); i++) {
				if (m_Renderer->Selected != i) continue;

				ImGui::Text("General Settings");
				std::string name = m_PointerToObjectsArray->at(i)->GetName();
				if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
					m_PointerToObjectsArray->at(i)->SetName(name);

				ImGui::Text("Transform");
				glm::vec3 _pos = m_PointerToObjectsArray->at(i)->GetPosition();
				float pos[3] = { _pos.x, _pos.y, _pos.z };
				ImGui::InputFloat3("Position", pos);
				m_PointerToObjectsArray->at(i)->SetPosition(glm::vec3(pos[0], pos[1], pos[2]));

				glm::vec3 _scale = m_PointerToObjectsArray->at(i)->GetScale();
				float scale[3] = { _scale.x, _scale.y, _scale.z };
				ImGui::InputFloat3("Scale", scale);
				m_PointerToObjectsArray->at(i)->SetScale(glm::vec3(scale[0], scale[1], scale[2]));

				glm::vec3 _rotation = m_PointerToObjectsArray->at(i)->GetRotation();
				float rotation[3] = { _rotation.x, _rotation.y, _rotation.z };
				ImGui::InputFloat3("Rotation", rotation);
				m_PointerToObjectsArray->at(i)->SetRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));

				ImGui::Text("Rendering");
				glm::vec4 _col = m_PointerToObjectsArray->at(i)->GetColor();
				float col[4] = { _col.x, _col.y, _col.z, _col.w };
				float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.50f;
				ImGui::SetNextItemWidth(w);
				ImGui::ColorPicker4("Color", col, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
				m_PointerToObjectsArray->at(i)->SetColor(glm::vec4(col[0], col[1], col[2], col[3]));
			}
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