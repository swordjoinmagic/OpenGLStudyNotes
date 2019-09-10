#version 330 core
out vec4 fragColor;
in vec2 uv;

uniform sampler2D MainTex;

void main(){    
    vec4 texColor = texture(MainTex,uv);
    if(texColor.a<0.1)
        discard;
    fragColor = texColor;
}