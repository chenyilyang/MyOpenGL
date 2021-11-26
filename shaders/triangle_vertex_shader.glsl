#version 330 core
layout(location = 0) in vec3 aPos;
out vec4 _fragColor;
void main() {
    gl_Position = vec4(aPos, 1.0);
    _fragColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);//dark red fragment color
}