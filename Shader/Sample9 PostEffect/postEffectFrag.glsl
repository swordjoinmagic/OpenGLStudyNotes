#version 330 core
out vec4 fragColor;
in vec2 uv;

uniform sampler2D screenTex;

void main(){
    fragColor = texture(screenTex,uv);
}