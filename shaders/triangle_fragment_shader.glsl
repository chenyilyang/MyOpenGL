#version 330 core
out vec4 FragColor;
// in vec4 fragColor; //set fragment color from vertex color
uniform vec4 _fragColor;
void mian() {
    FragColor = _fragColor;
}