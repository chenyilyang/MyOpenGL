#version 330 core
out vec4 FragColor;
// in vec3 _fragColor;
// in vec2 TexCoord;
in vec3 _normal;
in vec3 _fragPos;

//Texture sampler
// uniform sampler2D texture1;
// uniform sampler2D texture2;
// uniform float mixValue;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float specularStrength = 0.5f;

void main() {
    //linearly interpolate between both textures(80% container, 20% awesomeface)
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue) * vec4(_fragColor, 1.0f);
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord * 2.0), mixValue);
    // FragColor = vec4(_fragColor, 1.0f);
    // FragColor = vec4(lightColor * objectColor, 1.0f);
    //ambient
    float ambientStrength = 0.1f;
    vec3 ambient = lightColor * ambientStrength;
    //diffuse
    vec3 norm = normalize(_normal);
    vec3 lightDir = normalize(lightPos - _fragPos);
    //specular
    vec3 viewDir = normalize(viewPos - _fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 64.0f);
    vec3 specular = specularStrength * spec * lightColor;

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffUse = diff * lightColor;
    // vec3 result = ambient * objectColor;
    vec3 result = (ambient + diffUse + specular) * objectColor;
    FragColor = vec4(result, 1.0f);
}