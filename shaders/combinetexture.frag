#version 330 core
out vec4 FragColor;
in vec3 _fragColor;
in vec2 TexCoord;

//Texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

void main() {
    //linearly interpolate between both textures(80% container, 20% awesomeface)
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2) * vec4(_fragColor, 1.0f);
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord * 2.0), mixValue);
}