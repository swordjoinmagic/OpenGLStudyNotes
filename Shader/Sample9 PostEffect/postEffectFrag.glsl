#version 330 core
out vec4 fragColor;
in vec2 uv;

uniform sampler2D screenTex;

void main(){
    fragColor = vec4(vec3(1.0-texture(screenTex,uv)),1.0);
}