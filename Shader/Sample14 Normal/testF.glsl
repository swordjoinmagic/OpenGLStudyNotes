#version 330 core
out vec4 fragColor;

in mat3 TBN;
in vec2 uv;
in vec3 worldPos;

uniform sampler2D MainTex;
uniform sampler2D NormalTex;
uniform vec3 lightDir;

void main(){
    // 从法线贴图中采样法线,并将法线从切线空间转换至世界空间下
    vec3 normal = texture(NormalTex,uv).rgb;
    // 映射到[-1,1]区间
    normal = normalize(normal * 2.0 - 1.0);
    vec3 worldNormal = normalize(TBN * normal);

    // 计算漫反射辐照度
    float NdotL = dot(worldNormal , normalize(lightDir));

    // 计算漫反射光照
    vec4 diffuse = vec4(texture(MainTex,uv).rgb * NdotL,1.0);
    // 环境光
    vec4 ambient = diffuse * 0.1;

    fragColor = diffuse + ambient;
}