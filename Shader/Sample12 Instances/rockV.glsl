#version 330 core
layout(location = 0) in vec3 vertexPos;
// layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;
// 同时占了3,4,5的位置,因为OpenGL输入变量最大为vec4
layout(location = 3) in mat4 instanceMatrix;

out vec2 uv;

uniform mat4 projection;
uniform mat4 view;

void main(){
    gl_Position = projection * view  * instanceMatrix * vec4(vertexPos,1.0);
    uv = texcoords;
}