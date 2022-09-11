#include "Renderer.h"
#include "../Util/Debug.h"

GameEngine::Renderer::Renderer(RendererInformation rendererInformation)
{
    m_RendererInformation = rendererInformation;

    if (!glfwInit())
    {
        DebugError("GLFW failed to initialize");
        return;
    }

    m_window = glfwCreateWindow(rendererInformation.width, rendererInformation.height,
                                rendererInformation.title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        DebugError("GLFW failed to create window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        DebugError("GLAD failed to initialize");
        glfwTerminate();
        return;
    }

    glViewport(0, 0, rendererInformation.width, rendererInformation.height);

    glEnable(GL_DEPTH);
}

void GameEngine::Renderer::CreateFramebuffer()
{
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    // create depth texture
    glGenRenderbuffers(1, &m_depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, RenderSize.x, RenderSize.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // create a color attachment texture
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RenderSize.x, RenderSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0);
}

void GameEngine::Renderer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameEngine::Renderer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned GameEngine::Renderer::GetTexture() const { return m_textureID; }

void GameEngine::Renderer::Resize(int width, int height)
{
    if (RenderSize.x == width && RenderSize.y == height) return;
    glViewport(0, 0, width, height);
    RenderSize.x = width;
    RenderSize.y = height;

    CreateFramebuffer();
}

void GameEngine::Renderer::UpdateDeltaTime()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    DeltaTime = currentFrame - m_LastFrame;
    m_LastFrame = currentFrame;
}

GLFWwindow* GameEngine::Renderer::GetWindow() const { return m_window; }
