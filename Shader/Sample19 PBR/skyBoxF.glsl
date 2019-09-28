#version 330 core
out vec4 fragColor;

in vec3 dir;

uniform samplerCube environmentMap;

void main(){
    vec3 envColor = texture(environmentMap,dir).rgb;
    
    // 色调映射(tone mapping)
    envColor = envColor / (envColor + vec3(1.0));
    // 伽马矫正
    envColor = pow(envColor,vec3(1.0/2.2));

    fragColor = vec4(envColor,1.0);
}