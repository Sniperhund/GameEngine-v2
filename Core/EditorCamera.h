#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Graphics/Renderer.h"

namespace GameEngine
{
    enum Camera_Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    // Default camera values
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float FOV = 45.0f;

    class Camera
    {
    private:
        float lastX, lastY;
        bool firstMouse = true;
        float MovementSpeed;
    public:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        float Yaw;
        float Pitch;

        float MovementSpeedFast;
        float MovementSpeedSlow;
        float MouseSensitivity;
        float Fov;

        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw = YAW, float pitch = PITCH) : MovementSpeed(SPEED), Front(glm::vec3(0.0f, 0.0f, -1.0f)),
                                                       MouseSensitivity(SENSITIVITY), Fov(FOV)
        {
            MovementSpeedSlow = MovementSpeed;
            MovementSpeedFast = MovementSpeed * 3;
            Position = position;
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }

        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
            MovementSpeed(SPEED), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MouseSensitivity(SENSITIVITY), Fov(FOV)
        {
            Position = glm::vec3(posX, posY, posZ);
            WorldUp = glm::vec3(upX, upY, upZ);
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }

        glm::mat4 GetViewMatrix()
        {
            return lookAt(Position, Position + Front, Up);
        }

        void HandleInput()
        {
            int state = glfwGetMouseButton(Renderer::GetWindow(), GLFW_MOUSE_BUTTON_RIGHT);
            if (state != GLFW_PRESS)
            {
                glfwSetInputMode(Renderer::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                firstMouse = true;
                return;
            }

            glfwSetInputMode(Renderer::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            double xpos, ypos;
            glfwGetCursorPos(Renderer::GetWindow(), &xpos, &ypos);

            if (firstMouse)
            {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }

            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos;

            lastX = xpos;
            lastY = ypos;

            ProcessMouseMovement(xoffset, yoffset);

            if (glfwGetKey(Renderer::GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                MovementSpeed = MovementSpeedFast;
            if (glfwGetKey(Renderer::GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
                MovementSpeed = MovementSpeedSlow;
            if (glfwGetKey(Renderer::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
                ProcessKeyboard(FORWARD, Renderer::DeltaTime);
            if (glfwGetKey(Renderer::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
                ProcessKeyboard(BACKWARD, Renderer::DeltaTime);
            if (glfwGetKey(Renderer::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
                ProcessKeyboard(LEFT, Renderer::DeltaTime);
            if (glfwGetKey(Renderer::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
                ProcessKeyboard(RIGHT, Renderer::DeltaTime);
            if (glfwGetKey(Renderer::GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
                ProcessKeyboard(UP, Renderer::DeltaTime);
            if (glfwGetKey(Renderer::GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                ProcessKeyboard(DOWN, Renderer::DeltaTime);
        }

        void ProcessKeyboard(Camera_Movement direction, float deltaTime)
        {
            float velocity = MovementSpeed * deltaTime;
            if (direction == FORWARD)
                Position += Front * velocity;
            if (direction == BACKWARD)
                Position -= Front * velocity;
            if (direction == LEFT)
                Position -= Right * velocity;
            if (direction == RIGHT)
                Position += Right * velocity;
            if (direction == UP)
                Position += WorldUp * velocity;
            if (direction == DOWN)
                Position -= WorldUp * velocity;
        }

        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
        {
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            Yaw += xoffset;
            Pitch += yoffset;

            if (constrainPitch)
            {
                if (Pitch > 89.0f)
                    Pitch = 89.0f;
                if (Pitch < -89.0f)
                    Pitch = -89.0f;
            }

            updateCameraVectors();
        }

    private:
        void updateCameraVectors()
        {
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = normalize(front);

            Right = normalize(cross(Front, WorldUp));
            Up = normalize(cross(Right, Front));
        }
    };
}
