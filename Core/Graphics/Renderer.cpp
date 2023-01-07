#include "Renderer.h"
#include "../Util/Debug.h"

GLFWwindow* GameEngine::Renderer::m_window;

unsigned int GameEngine::Renderer::m_framebuffer = 0;
unsigned int GameEngine::Renderer::m_textureID = 0;
unsigned int GameEngine::Renderer::m_depthbuffer = 0;

float GameEngine::Renderer::m_LastFrame;

glm::mat4 GameEngine::Renderer::Projection;
glm::mat4 GameEngine::Renderer::View;

float GameEngine::Renderer::RenderTimeInMs;
float GameEngine::Renderer::DeltaTime;

int GameEngine::Renderer::Selected = -1;

glm::vec2 GameEngine::Renderer::RenderSize;

void GameEngine::Renderer::Init(int width, int height, std::string title)
{
    RenderSize.x = width;
    RenderSize.y = height;
    if (!glfwInit())
    {
        DebugError("GLFW failed to initialize");
        return;
    }

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    m_window = glfwCreateWindow(width, height,
                                title.c_str(), nullptr, nullptr);
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

    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    } 
}

void GameEngine::Renderer::CreateFramebuffer()
{
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    // create depth texture
    glGenRenderbuffers(1, &m_depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RenderSize.x, RenderSize.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // create a color attachment texture
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RenderSize.x, RenderSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
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

unsigned GameEngine::Renderer::GetTexture() { return m_textureID; }

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

GLFWwindow* GameEngine::Renderer::GetWindow() { return m_window; }

void glDebugOutput(GLenum source, GLenum type, unsigned id, GLenum severity, GLsizei length, const char* message,
    const void* userParam)
{
	// ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}
