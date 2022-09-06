#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec4 aIds;
layout (location = 6) in vec4 aWeights;

out vec3 Normal;
out vec2 TexCoords;
out vec3 Tangent;
out vec3 Bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	TexCoords = aTexCoords;
    Normal = aNormal;
    Tangent = aTangent;
    Bitangent = aBitangent;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}