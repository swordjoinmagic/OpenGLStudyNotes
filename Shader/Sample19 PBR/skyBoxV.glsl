#version 330 core
layout(location = 0) in vec3 vertexPos;

out vec3 dir;

uniform mat4 projection;
uniform mat4 view;

void main(){
    dir = vertexPos;
    // 去除观察矩阵的平移
    mat4 rotView = mat4(mat3(view));
    vec4 clipPos = projection * rotView * vec4(vertexPos,1.0);

    // 保证渲染的片段的深度值总是1.0(即最远处)
    // 这样就可以将天空盒放到最后渲染,减少overdraw
    gl_Position = clipPos.xyww;
}