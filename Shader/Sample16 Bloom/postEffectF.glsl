#version 330 core
in vec2 uv;
out vec4 fragColor;

uniform sampler2D ScreenTex;
uniform sampler2D blurTex;

void main(){
    fragColor = texture(ScreenTex,uv) + texture(blurTex,uv);
}