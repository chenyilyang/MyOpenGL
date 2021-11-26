#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
uniform float anim_time;
out vec3 _fragColor;
void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    _fragColor = vec3(aColor.r * anim_time, aColor.g * anim_time, aColor.b * anim_time);
}