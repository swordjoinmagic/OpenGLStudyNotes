#version 330 core
layout(points) in;
layout(triangle_strip,max_vertices=5) out;

void build_house(vec4 position){
    gl_Position = position + vec4(-0.2,-0.2,0,0);   // 左下
    EmitVertex();
    gl_Position = position + vec4(0.2,-0.2,0.0,0.0);    // 右下
    EmitVertex();
    gl_Position = position + vec4(-0.2,0.2,0,0);    // 左上
    EmitVertex();
    gl_Position = position + vec4(0.2,0.2,0.0,0.0); // 右上
    EmitVertex();
    gl_Position = position + vec4(0.0,0.4,0.0,0.0); // 顶部
    EmitVertex();
    EndPrimitive();
}

void main(){
    build_house(gl_in[0].gl_Position);
}