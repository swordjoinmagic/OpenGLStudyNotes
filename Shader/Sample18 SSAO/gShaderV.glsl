#version 330 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 viewFragPos;
out vec3 viewFragNormal;
out vec2 uv;

void main(){
    gl_Position = projection * view * model * vec4(vertexPos,1.0);
    // 存储观察空间的物体坐标和法线
    viewFragPos = (view * model * vec4(vertexPos,1.0)).xyz;
    viewFragNormal = transpose(inverse(mat3(view * model))) * normal;
    uv = texcoords;
}