#version 330 core
out vec4 fragColor;

in vec3 worldPos;
in vec3 worldNormal;

uniform samplerCube skybox;
uniform vec3 viewPos;

void main(){
    vec3 normal = normalize(worldNormal);
    // 折射率(光线冲空气进入玻璃)
    float ratio = 1.00 / 1.52;
    vec3 viewDir = normalize(worldPos - viewPos);
    vec3 refractDir = normalize( refract(viewDir,normal,ratio) );
    fragColor = vec4(texture(skybox,refractDir).rgb,1.0);
}