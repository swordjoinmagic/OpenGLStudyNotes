#version 330 core
layout(location = 0) out vec3 gPos;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gAldebo;

in vec3 worldPos;
in vec3 worldNormal;
in vec2 uv;

uniform sampler2D diffuseTex;

void main(){
    gPos = worldPos;
    gNormal = worldNormal;
    gAldebo = (texture(diffuseTex,uv)).rgb;
}