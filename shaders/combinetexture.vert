#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform float anim_time;

out vec3 _fragColor;
out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    _fragColor = vec3(aColor.r * (1.0 - anim_time), aColor.g * (anim_time), aColor.r * ((1.0 - anim_time) / 2.0));
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}