#pragma once

#include <xstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace GameEngine
{
    struct RendererInformation
    {
        int width;
        int height;
        std::string title;

        RendererInformation(int _width = 800, int _height = 600, std::string _title = "GameEngine") :
            width(_width), height(_height), title(_title)
        {
        }
    };

    class Renderer
    {
    private:
        GLFWwindow* m_window;

        RendererInformation m_RendererInformation;

        unsigned int m_framebuffer = 0;
        unsigned int m_textureID = 0;
        unsigned int m_depthbuffer = 0;

        float m_LastFrame;
    public:
        glm::mat4 Projection;
        glm::mat4 View;

        float RenderTimeInMs;
        float DeltaTime;

        int Selected = -1;

        glm::vec2 RenderSize;
        Renderer(RendererInformation rendererInformation);

        void CreateFramebuffer();
        void Bind();
        void Unbind();
        unsigned int GetTexture() const;

        void Resize(int width, int height);

        void UpdateDeltaTime();

        GLFWwindow* GetWindow() const;
    };
}
