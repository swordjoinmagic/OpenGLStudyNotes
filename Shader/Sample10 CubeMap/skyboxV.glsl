#version 330 core
layout(location=0) in vec3 vertexPos;

out vec3 dir;

uniform mat4 view;
uniform mat4 projection;

void main(){
    dir = vertexPos;
    gl_Position = projection * view * vec4(vertexPos,1.0);
}