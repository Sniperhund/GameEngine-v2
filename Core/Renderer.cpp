#include "Renderer.h"
#include "Debug.h"

GameEngine::Renderer::Renderer(RendererInformation rendererInformation)
{
	m_RendererInformation = rendererInformation;

	if (!glfwInit()) {
		DebugError("GLFW failed to initialize");
		return;
	}

	m_window = glfwCreateWindow(rendererInformation.width, rendererInformation.height,
		rendererInformation.title.c_str(), NULL, NULL);
	if (!m_window) {
		DebugError("GLFW failed to create window");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		DebugError("GLAD failed to initialize");
		glfwTerminate();
		return;
	}

	glViewport(0, 0, rendererInformation.width, rendererInformation.height);
	glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
}
