#pragma once

#include <xstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
	public:
		Renderer(RendererInformation rendererInformation);

		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
		}
	};
}
