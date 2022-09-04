#pragma once

#include <xstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace GameEngine {
	struct RendererInformation {
		int width;
		int height;
		std::string title;

		RendererInformation(int _width = 800, int _height = 600, std::string _title = "GameEngine") :
			width(_width), height(_height), title(_title) {}
	};

	class Renderer
	{
	private:
		GLFWwindow* m_window;

		RendererInformation m_RendererInformation;

		unsigned int m_framebuffer = 0;
		unsigned int m_textureID = 0;
	public:
		glm::vec2 RenderSize;
		Renderer(RendererInformation rendererInformation);

		void CreateFramebuffer();
		void Bind();
		void Unbind();
		unsigned int GetTexture() { return m_textureID; }

		void Resize(int width, int height) {
			if (RenderSize.x == width && RenderSize.y == height) return;
			glViewport(0, 0, width, height);
			RenderSize.x = width;
			RenderSize.y = height;

			CreateFramebuffer();
		}

		GLFWwindow* GetWindow() { return m_window; }
	};
}
