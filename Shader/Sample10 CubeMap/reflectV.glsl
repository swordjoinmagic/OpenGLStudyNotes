#version 330 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 worldNormal;
out vec3 worldPos;

void main(){
    gl_Position = projection * view * model * vec4(vertexPos,1.0);
    worldNormal = normalize( mat3(transpose(inverse(model))) * normal );
    worldPos = (model * vec4(vertexPos,1.0)).xyz;
}