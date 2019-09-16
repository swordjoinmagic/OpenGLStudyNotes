#version 330 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 Texcoords;

uniform mat4 model;

void main(){
    // 在顶点着色器阶段将顶点从模型空间转换至世界空间下
    gl_Position = model * vec4(vertexPos,1);
}