#version 330 core
in vec2 uv;
out vec4 fragColor;

uniform sampler2D MainTex;

void main(){
    fragColor = vec4(texture(MainTex,uv).r);
}