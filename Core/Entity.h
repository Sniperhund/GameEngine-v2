#pragma once

#include <glm/glm.hpp>
#include <format>

#include "EditorCamera.h"
#include "Shader.h"
#include "Model.h"
#include "Renderer.h"

namespace GameEngine {
	class Entity {
	protected:
		glm::vec3 m_Position = glm::vec3(0);
		glm::vec3 m_Scale = glm::vec3(0);
		glm::vec3 m_Rotation = glm::vec3(0);

		Renderer* m_Renderer;
		Camera* m_CurrentCamera;
	public:
		virtual void _Start(Renderer* renderer) {
			m_Renderer = renderer;
			Start();
		}

		virtual void _Update(Camera* currentCamera) {
			m_CurrentCamera = currentCamera;
			Update();
		}

		virtual void Start() {}
		virtual void Update() {}
	};

	class Object {
	private:
		bool m_IsInit = false;

		Shader m_Shader;
		Model m_Model;

		std::string m_ShaderPath = "";
		std::string m_ModelPath = "";

		glm::vec3 m_Position = glm::vec3(0);
		glm::vec3 m_Scale = glm::vec3(1);
		glm::vec3 m_Rotation = glm::vec3(0);
		glm::vec4 m_Color = glm::vec4(0);

		Renderer* m_Renderer;
		Camera* m_CurrentCamera;
	public:
		std::string Name = "Default Name";

		void SetPosition(glm::vec3 position) { m_Position = position; }
		void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; }
		void SetScale(glm::vec3 scale) { m_Scale = scale; }
		void SetColor(glm::vec4 color) { m_Color = color; }
		glm::vec3 GetPosition() { return m_Position; }
		glm::vec3 GetRotation() { return m_Rotation; }
		glm::vec3 GetScale() { return m_Scale; }
		glm::vec4 GetColor() { return m_Color; }

		void _Start(Renderer* renderer) {
			if (m_IsInit) return;

			m_IsInit = true;
			m_Renderer = renderer;
			Start();
		}

		void _Update(Camera* currentCamera) {
			m_CurrentCamera = currentCamera;
			Update();
		}

		virtual void SetInfo(std::string shaderPath, std::string modelPath) {
			m_ShaderPath = shaderPath;
			m_ModelPath = modelPath;
		}

		virtual void Start()
		{
			if (m_ShaderPath.empty() && m_ModelPath.empty()) {
				DebugError("Something went wrong. Somehow someone forgot to set the model path or shader path");
			}

			m_Shader.Init(std::format("{}.vert", m_ShaderPath).c_str(), std::format("{}.frag", m_ShaderPath).c_str());
			m_Model.Init(m_ModelPath);
		}

		virtual void Update()
		{
			if (m_Renderer->RenderSize.x >= 1 && m_Renderer->RenderSize.y >= 1) {
				m_Shader.Use();

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, m_Position);
				model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
				model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
				model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
				model = glm::scale(model, m_Scale);
				m_Shader.SetMat4("model", model);

				glm::mat4 projection = glm::perspective(glm::radians(m_CurrentCamera->Fov),
					(float)m_Renderer->RenderSize.x / (float)m_Renderer->RenderSize.y, 0.1f, 100.0f);
				m_Shader.SetMat4("projection", projection);
				glm::mat4 view = m_CurrentCamera->GetViewMatrix();
				m_Shader.SetMat4("view", view);

				m_Shader.SetVec4("custom_color", m_Color);

				m_Model.Draw(m_Shader);
			}
		}
	};
}