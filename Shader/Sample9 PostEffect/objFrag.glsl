#version 330 core
out vec4 fragColor;
in vec2 uv;

uniform sampler2D MainTex;

void main(){
    fragColor = vec4(1,0,0,1);
}