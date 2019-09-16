#version 330 core

in vec4 worldPos;

// 点光源位置
uniform vec3 lightPos;
// 光源视椎体的远平面距离,用于将深度线性化
uniform float far_plane;
uniform sampler2D MainTex;

void main(){
    // 片元距离光源的距离
    float lightDistance =  length(worldPos.xyz - lightPos);

    // 将深度变换为[0,1]区间
    lightDistance = lightDistance / far_plane;

    // 配置该点片元的深度
    gl_FragDepth = lightDistance;

    vec4 fragColor = texture(MainTex,vec2(0,0));
}