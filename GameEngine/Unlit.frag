#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 diffuse;
uniform vec4 custom_color;
uniform bool is_texture_used;
uniform sampler2D texture_diffuse1;

void main() {
    vec4 result;
    if (is_texture_used)
    result += texture(texture_diffuse1, TexCoords);
    else
    result += vec4(diffuse, 1.0);
    FragColor = mix(result, custom_color, custom_color.w);
}