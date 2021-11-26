#version 330 core
out vec4 FragColor;
in vec3 _fragColor;
void main() {
    FragColor = vec4(_fragColor, 1.0f);
}