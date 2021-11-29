#version 330 core
out vec4 FragColor;
in vec3 _fragColor;
in vec2 TexCoord;

//Texture sampler
uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, TexCoord) * vec4(_fragColor, 1.0);
}