#version 330 core
layout(triangles) in;
layout(triangle_strip,max_vertices = 18) out;

uniform mat4 shadowMatrices[6];

out vec4 worldPos;

void main(){
    for(int face = 0;face < 6;face++){
        gl_Layer = face;
        // 将输入片元转换至立方体贴图的六个面
        for(int i=0;i<3;i++){
            worldPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * worldPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}