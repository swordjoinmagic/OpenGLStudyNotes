#version 330 core
out vec4 fragColor;

in vec3 worldPos;
in vec3 worldNormal;

uniform samplerCube skybox;
uniform vec3 viewPos;

void main(){
    vec3 worldViewDir = normalize(worldPos - viewPos);
    vec3 normal = normalize(worldNormal);

    vec3 reflectDir = reflect(worldViewDir,normal);

    fragColor = vec4(texture(skybox,reflectDir).rgb,1.0);
}