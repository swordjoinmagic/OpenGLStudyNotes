#version 330 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;
layout(location = 3) in vec3 tangent;

out VS_OUT{
    vec3 worldPos;
    vec2 uv;
    // 在切线空间下的光源位置
    vec3 tangentLightPos;
    // 切线空间下的观察位置
    vec3 tangentViewPos;
    // 切线空间下的世界坐标
    vec3 tangentFragPos;
} v2f;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
    gl_Position = projection * view * model * vec4(vertexPos,1.0);
    v2f.worldPos = vec3(model * vec4(vertexPos,1));
    v2f.uv = texcoords;

    // 副切线
    vec3 bitangent = cross(normal,tangent);

    // 构建 世界-切线 变换矩阵
    vec3 T = normalize( mat3(model) * tangent );
    vec3 B = normalize( mat3(model) * bitangent );
    vec3 N = normalize( mat3(model) * normal );
    mat3 TBN = transpose(mat3(T,B,N));

    v2f.tangentLightPos = TBN * lightPos;
    v2f.tangentViewPos = TBN * viewPos;
    v2f.tangentFragPos = TBN * v2f.worldPos;
}