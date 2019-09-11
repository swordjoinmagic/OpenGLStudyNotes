#version 330 core
layout(triangles) in;
layout(triangle_strip,max_vertices=3) out;
in VS_OUT{
    vec2 texcoords;
}gs_in[];

out vec2 uv;

uniform float time;

// 将顶点跟着法线方向外拓
vec4 explode(vec4 position,vec3 normal){
    float manitude = 2.0;
    vec3 direction = normal * ((sin(time)+1.0)/2.0) * manitude;
    return position + vec4(direction,0.0);
}

vec3 GetNormal(){
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a,b));
}

void main(){
    vec3 normal = GetNormal();
    gl_Position = explode(gl_in[0].gl_Position,normal);
    uv = gs_in[0].texcoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position,normal);
    uv = gs_in[1].texcoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position,normal);
    uv = gs_in[2].texcoords;
    EmitVertex();
    EndPrimitive();
}