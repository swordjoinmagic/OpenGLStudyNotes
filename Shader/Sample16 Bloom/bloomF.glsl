#version 330 core
// 渲染正常场景的
layout(location = 0) out vec4 fragColor;
// 提取亮色的
layout(location = 1) out vec4 brightColor;
in vec2 uv;

uniform sampler2D MainTex;

void main(){
    fragColor = texture(MainTex,uv);

    // 获得亮度值(灰度值)
    float brightness = dot(fragColor.rgb,vec3(0.2126,0.7152,0.0722));
    if(brightness > 0.8)
        brightColor = vec4(fragColor.rgb,1.0);
}