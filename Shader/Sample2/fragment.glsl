#version 330 core
out vec4 fragColor;

in vec3 ourColor;
in vec2 uv;

uniform sampler2D ourTexture;

void main(){
    fragColor = texture(ourTexture,uv) * ourColor;
}