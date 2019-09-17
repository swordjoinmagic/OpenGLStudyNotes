#version 330 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;
layout(location = 3) in vec3 tangent;
// layout(location = 4) in vec3 bitangle;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out mat3 TBN;
out vec2 uv;
out vec3 worldPos;

void main(){
    gl_Position = projection * view * model * vec4(vertexPos,1.0);

    vec3 bitangle = cross(tangent,normal);

    // 创建TBN矩阵
    vec3 T = normalize(vec3(model * vec4(tangent,0.0)));
    vec3 B = normalize(vec3(model * vec4(bitangle,0.0)));
    vec3 N = normalize(vec3(model * vec4(normal,0.0)));

    TBN = mat3(T,B,N);
    uv = texcoords;
    worldPos = (model*vec4(vertexPos,1.0)).xyz;
}