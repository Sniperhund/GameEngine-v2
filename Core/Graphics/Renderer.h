#pragma once

#include <xstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace GameEngine
{
    class Renderer
    {
    private:
        static GLFWwindow* m_window;

        static unsigned int m_framebuffer;
        static unsigned int m_textureID;
        static unsigned int m_depthbuffer;

        static float m_LastFrame;
    public:
        static glm::mat4 Projection;
        static glm::mat4 View;

        static float RenderTimeInMs;
        static float DeltaTime;

        static int Selected;

        static glm::vec2 RenderSize;
        static void Init(int width, int height, std::string title);

        static void CreateFramebuffer();
        static void Bind();
        static void Unbind();
        static unsigned int GetTexture();

        static void Resize(int width, int height);
        
        static void UpdateDeltaTime();

        static GLFWwindow* GetWindow();
    };
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
			GLsizei length, const char *message, const void *userParam);