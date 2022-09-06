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
		Shader m_Shader;
		Model m_Model;

		std::string m_ShaderPath = "";
		std::string m_ModelPath = "";

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

		void SetInfo(std::string shaderPath, std::string modelPath) {
			m_ShaderPath = shaderPath;
			m_ModelPath = modelPath;
		}

		void Start()
		{
			if (!m_ShaderPath.empty() && !m_ModelPath.empty()) {
				DebugError("Something went wrong. Somehow someone forgot to set the model path or shader path");
			}

			m_Shader.Init(std::format("{}.vert", m_ShaderPath).c_str(), std::format("{}.frag", m_ShaderPath).c_str());
			m_Model.Init(m_ModelPath);
		}

		void Update()
		{
			if (m_Renderer->RenderSize.x >= 1 && m_Renderer->RenderSize.y >= 1) {
				m_Shader.Use();

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(0, 0, 0));
				m_Shader.SetMat4("model", model);

				glm::mat4 projection = glm::perspective(glm::radians(m_CurrentCamera->Fov),
					(float)m_Renderer->RenderSize.x / (float)m_Renderer->RenderSize.y, 0.1f, 100.0f);
				m_Shader.SetMat4("projection", projection);
				glm::mat4 view = m_CurrentCamera->GetViewMatrix();
				m_Shader.SetMat4("view", view);

				m_Model.Draw(m_Shader);
			}
		}
	};
}