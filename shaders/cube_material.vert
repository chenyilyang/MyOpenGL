#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
// layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

// uniform float anim_time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

out vec3 _fragColor;
out vec3 _normal;
out vec3 _fragPos;
// out vec2 TexCoord;

void main() {
    _fragColor = color;
    _fragPos = vec3(model * vec4(aPos, 1.0f));
    // _normal = aNormal;
    _normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * vec4(_fragPos, 1.0f);
    // _fragColor = vec3(aColor);
    // _fragColor = vec3(aColor.r * (1.0 - anim_time), aColor.g * (anim_time), aColor.r * ((1.0 - anim_time) / 2.0));
    // TexCoord = vec2(aTexCoord.x, aTexCoord.y) * -1;//flip the texture coordination
}