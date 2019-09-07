// 用于点光源光照的Shader
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv;
out vec3 worldNormal;
out vec3 worldPos;

void main(){
    gl_Position = projection * view * model * vec4(pos,1);
    worldPos = vec3( model * vec4(pos,1) );
    worldNormal = normalize(mat3(transpose(inverse(model))) * normal);
    uv = texcoord;
}