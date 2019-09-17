#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 worldPos;
    vec2 uv;
    // 在切线空间下的光源位置
    vec3 tangentLightPos;
    // 切线空间下的观察位置
    vec3 tangentViewPos;
    // 切线空间下的世界坐标
    vec3 tangentFragPos;
} v2f;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float height_scale;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

void main(){
    // 切线空间下的观察方向
    vec3 viewDir = normalize( v2f.tangentViewPos - v2f.tangentFragPos );
    vec2 uv = ParallaxMapping(v2f.uv,viewDir);

    // 从法线贴图中获取法线(切线空间下)
    vec3 normal = texture(normalMap,uv).xyz;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 lightDir = normalize(v2f.tangentLightPos - v2f.tangentFragPos);

    // 计算漫反射光照
    float NdotL = max(0,dot(normal,lightDir));
    vec4 diffuse = texture(diffuseMap,uv) * NdotL;
    
    FragColor = diffuse;
}

// 根据观察方向,计算uv偏移
vec2 ParallaxMapping(vec2 texCoords,vec3 viewDir){
    // 获取当前片元的高度
    float height = texture(depthMap,texCoords).r;
    // 根据片元高度,观察方向获得偏移
    // 这里之所以要/z,是因为要根据视角来变换偏移程度
    // z越接近1,说明观察方向垂直于平面,偏移程度越小,
    // z越接近0,说明观察方向与平面夹角越大,偏移程度越大
    vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
    return texCoords + p;
}