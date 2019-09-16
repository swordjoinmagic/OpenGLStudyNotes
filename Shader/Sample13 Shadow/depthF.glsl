#version 330 core
out vec4 fragColor;
in vec2 uv;
in vec3 worldNormal;
in vec3 worldPos;

uniform sampler2D MainTex;

void main(){
    fragColor = texture(MainTex,uv);
}