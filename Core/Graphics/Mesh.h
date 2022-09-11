#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

#include <vector>
#include <xstring>

namespace GameEngine
{
#define MAX_BONE_INFLUENCE 4

    struct Material
    {
        glm::vec3 Diffuse = glm::vec3(0.52, 0.52, 0.52);
        glm::vec3 Specular;
        glm::vec3 Ambient;
        float Shininess;
    };

    struct Vertex
    {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
        //bone indexes which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    struct Texture
    {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh
    {
    public:
        // mesh Data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        unsigned int VAO{};

        // constructor
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        // render the mesh
        void Draw(Shader& shader, Material mat);

    private:
        // render data 
        unsigned int VBO{}, EBO{};

        // initializes all the buffer objects/arrays
        void setupMesh();
    };
}
